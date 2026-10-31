
#include "AudioEngineImpl_MA.h"
#include "Common_MA.h"
#include "Config_MA.h"
#include "FileIOVFS_MA.h"
#include "ATLData.h"

#include <AzCore/Debug/Profiler.h>
#include <AzCore/StringFunc/StringFunc.h>
#include <AzCore/std/string/conversions.h>

//MiniAudio Headers:
#include <extras/decoders/libopus/miniaudio_libopus.h>
#include <extras/decoders/libvorbis/miniaudio_libvorbis.h>

using namespace Audio;

namespace AudioEngineMA
{

namespace MemHooks
{
    void* AzMalloc(size_t size, void* pUserData)
    {
        AZ_UNUSED(pUserData);
        return AZ::AllocatorInstance<Audio::AudioImplAllocator>::Get().Allocate(size, 0, 0, nullptr);
    }

    void* AzRealloc(void *mem, size_t size, void* pUserData)
    {
        AZ_UNUSED(pUserData);
        return AZ::AllocatorInstance<Audio::AudioImplAllocator>::Get().ReAllocate(mem, size, 0);
    }

    void AzMemFree(void *mem, void* pUserData)
    {
        AZ_UNUSED(pUserData);
        AZ::AllocatorInstance<Audio::AudioImplAllocator>::Get().DeAllocate(mem);
    }

}

namespace Logging
{
    void AzLogCallback(void* pUserData, ma_uint32 logLevel, const char* pMessage)
    {
        switch(logLevel)
        {
        case MA_LOG_LEVEL_ERROR:
            AZ_Error(Constants::LogWindow, false, pMessage);
            break;
        case MA_LOG_LEVEL_WARNING:
            AZ_Warning(Constants::LogWindow, false, pMessage);
            break;
        case MA_LOG_LEVEL_INFO:
            AZ_Info(Constants::LogWindow, pMessage);
            break;
        case MA_LOG_LEVEL_DEBUG:
            AZ_Trace(Constants::LogWindow, pMessage);
            break;
        default: break; //This shouldn't happen.
        }
    }
}

AudioSystemImpl_MA::AudioSystemImpl_MA()
{
    AudioSystemImplementationRequestBus::Handler::BusConnect();
    AudioSystemImplementationNotificationBus::Handler::BusConnect();
}

AudioSystemImpl_MA::~AudioSystemImpl_MA()
{
    AudioSystemImplementationRequestBus::Handler::BusDisconnect();
    AudioSystemImplementationNotificationBus::Handler::BusDisconnect();
}

void AudioSystemImpl_MA::Update(float updateIntervalMS)
{
    AZ_UNUSED(updateIntervalMS);
}

EAudioRequestStatus AudioSystemImpl_MA::Initialize()
{
    ma_allocation_callbacks azMemCallbacks= {
        .pUserData = nullptr,
        .onMalloc  = MemHooks::AzMalloc,
        .onRealloc = MemHooks::AzRealloc,
        .onFree    = MemHooks::AzMemFree
    };

    m_log = AZStd::make_unique<ma_log>();
    ma_result logres = ma_log_init(&azMemCallbacks, m_log.get());
    if(logres != MA_SUCCESS)
    {
        return EAudioRequestStatus::Failure;
    }
    ma_log_callback logCb = ma_log_callback_init(Logging::AzLogCallback, nullptr);
    ma_log_register_callback(m_log.get(), logCb);

    m_resourceManager = AZStd::make_unique<ma_resource_manager>();
    ma_resource_manager_config resmgr_config = ma_resource_manager_config_init();

    resmgr_config.allocationCallbacks = azMemCallbacks;
    ma_decoding_backend_vtable* customBackends[] =
    {
        ma_decoding_backend_libvorbis,
    };

    ma_vfs_callbacks vfsCb = {
        .onOpen  = FileIO::FileIO_Open,
        .onOpenW = nullptr,
        .onClose = FileIO::FileIO_Close,
        .onRead  = FileIO::FileIO_Read,
        .onWrite = nullptr,
        .onSeek  = FileIO::FileIO_Seek,
        .onTell  = FileIO::FileIO_Tell,
        .onInfo  = FileIO::FileIO_Info
    };

    resmgr_config.ppCustomDecodingBackendVTables = customBackends;
    resmgr_config.customDecodingBackendCount = AZ_ARRAY_SIZE(customBackends);
    resmgr_config.pCustomDecodingBackendUserData = nullptr; // could pass `this` in, its passed into each call to res man
    ma_result result = ma_resource_manager_init(&resmgr_config, m_resourceManager.get());
    if(result != MA_SUCCESS)
    {
        AZ_Error(Constants::LogWindow, false, "Failed to initialize MiniAudio resources manager, error: %d", result);
        return EAudioRequestStatus::Failure;
    }

    ma_engine_config engineCfg = ma_engine_config_init();
    m_engine = AZStd::make_unique<ma_engine>();
    engineCfg.pResourceManager = m_resourceManager.get();
    engineCfg.allocationCallbacks = azMemCallbacks;
    engineCfg.pResourceManagerVFS = reinterpret_cast<ma_vfs*>(&vfsCb);
    engineCfg.pLog = m_log.get();
    engineCfg.channels = 2;
    engineCfg.listenerCount = MA_ENGINE_MAX_LISTENERS;

    if(ma_result eng_res = ma_engine_init(&engineCfg, m_engine.get());
            eng_res != MA_SUCCESS)
    {
        ma_resource_manager_uninit(m_resourceManager.get());
        m_resourceManager.reset();
        m_engine.reset();

        AZ_Error(Constants::LogWindow, false, "Failure initializing MiniAudio: %d", eng_res);
        return EAudioRequestStatus::Failure;
    }

    //The listener #0 is reserved for the default listener.
    for(int i = MA_ENGINE_MAX_LISTENERS-1; i>=1; --i)
    {
        m_availableListeners.push_back(i);
    }

    m_soundGroupsManager = AZStd::make_unique<SoundGroupManager>(m_engine.get());
    AZ_Info(Constants::LogWindow, "MiniAudio-backend for AudioSystem initalialized");
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::ShutDown()
{
    ma_engine_uninit(m_engine.get());
    ma_resource_manager_uninit(m_resourceManager.get());
    ma_log_uninit(m_log.get());

    m_engine.reset();
    m_resourceManager.reset();
    m_log.reset();

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::Release()
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::StopAllSounds()
{
    for(auto& object : m_audioObjects)
    {
        for(auto& activeSnd : object->m_activeMASounds)
        {
            ma_sound_stop(activeSnd.second.m_sound);
            ma_sound_uninit(activeSnd.second.m_sound);
            azdestroy(activeSnd.second.m_sound, Audio::AudioImplAllocator, ma_sound);

            ma_decoder_uninit(activeSnd.second.m_decoder);
            azdestroy(activeSnd.second.m_sound, Audio::AudioImplAllocator, ma_decoder);
        }
        object->m_activeMASounds.clear();
    }

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::RegisterAudioObject(Audio::IATLAudioObjectData *objectData, const char *objectName)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::UnregisterAudioObject(Audio::IATLAudioObjectData *objectData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::ResetAudioObject(Audio::IATLAudioObjectData *objectData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::UpdateAudioObject(Audio::IATLAudioObjectData *objectData)
{
    AZ_PROFILE_FUNCTION(Audio);
    if(!objectData)
    {
        return EAudioRequestStatus::Failure;
    }

    //TODO: This should use a global deletion queue instead of deleting in the same object scope.?
    CheckObjectForExpiredMASounds(*static_cast<SATLAudioObjectData_MA*>(objectData));
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::PrepareTriggerSync(Audio::IATLAudioObjectData *audioObjectData, const Audio::IATLTriggerImplData *triggerData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::UnprepareTriggerSync(Audio::IATLAudioObjectData *objectData, const Audio::IATLTriggerImplData *triggerData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::PrepareTriggerAsync(Audio::IATLAudioObjectData *objectData, const Audio::IATLTriggerImplData *triggerData, Audio::IATLEventData *eventData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::UnprepareTriggerAsync(Audio::IATLAudioObjectData *pAudioObjectData, const Audio::IATLTriggerImplData *pTriggerData, Audio::IATLEventData *pEventData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::ActivateTrigger(Audio::IATLAudioObjectData *objectData, const Audio::IATLTriggerImplData *triggerData, Audio::IATLEventData *eventData, const Audio::SATLSourceData *sourceData)
{
    AZ_Info(Constants::LogWindow, "Trying Activating Trigger");
    auto object   = static_cast<SATLAudioObjectData_MA*>(objectData);
    auto trigger  = static_cast<const SATLTriggerImplData_MA*>(triggerData);
    auto event    = static_cast<SATLEventData_MA*>(eventData);

    if(!object || !trigger || !event)
    {
        AZ_Warning(Constants::LogWindow, false, "Activating Trigger becaause one of the following objects failed typecast: object, trigger or event");
        return EAudioRequestStatus::Failure;
    }

    auto audioSrcIt = m_audioSources.find(trigger->m_audioFilePath);
    if(audioSrcIt == m_audioSources.end()) {
        AZ_Warning(Constants::LogWindow, false, "We coudln't find audio source: %s, did you forgot to put the audio file into a preload control?", trigger->m_audioFilePath.c_str());
        return EAudioRequestStatus::Failure;
    }

    AZ_Info(Constants::LogWindow, "Trigger executed!: %s", trigger->m_audioFilePath.c_str());
    auto audioFilePath = audioSrcIt->first;

    switch(trigger->m_audioTriggerParams.m_soundAction)
    {
    case SoundAction::Start: {
        ma_decoder* decoder = azcreate(ma_decoder, (), Audio::AudioImplAllocator);
        ma_sound* sound = azcreate(ma_sound, (), Audio::AudioImplAllocator);

        ma_result res = ma_decoder_init_memory(audioSrcIt->second.first, audioSrcIt->second.second, nullptr, decoder);
        if(res != MA_SUCCESS)
        {
            AZ_Error(Constants::LogWindow, false, "Error trying to initialize decoder instance for '%s', code: %d", trigger->m_audioFilePath.c_str(), res);
            azdestroy(decoder, Audio::AudioImplAllocator, ma_decoder);
            azdestroy(sound, Audio::AudioImplAllocator, ma_sound);

            return EAudioRequestStatus::Failure;
        }

        res = ma_sound_init_from_data_source(m_engine.get(), decoder, MA_SOUND_FLAG_DECODE, nullptr, sound); //@TODO: sound groups!

        if(res != MA_SUCCESS)
        {
            AZ_Error(Constants::LogWindow, false, "Error trying to initialize sound instance for '%s', code: %d", trigger->m_audioFilePath.c_str(), res);
            azdestroy(decoder, Audio::AudioImplAllocator, ma_decoder);
            azdestroy(sound, Audio::AudioImplAllocator, ma_sound);

            return EAudioRequestStatus::Failure;
        }

        ma_sound_seek_to_pcm_frame(sound, 0);
        {
            ma_sound_set_spatialization_enabled(sound, trigger->m_audioTriggerParams.m_spatialization ? MA_TRUE : MA_FALSE);
            ma_sound_set_attenuation_model(sound, AttenuationMode::ToMAModel(trigger->m_audioTriggerParams.m_attenuationMode));
            ma_sound_set_pan_mode(sound, PanMode::ToMA(trigger->m_audioTriggerParams.m_panMode));
            ma_sound_set_positioning(sound, PositioningMode::ToMA(trigger->m_audioTriggerParams.m_positioning));
            ma_sound_set_pan(sound, trigger->m_audioTriggerParams.m_pan);
            ma_sound_set_min_distance(sound, trigger->m_audioTriggerParams.m_minDistance);
            ma_sound_set_max_distance(sound, trigger->m_audioTriggerParams.m_maxDistance);
            ma_sound_set_cone(sound, trigger->m_audioTriggerParams.m_coneinnerAngle, trigger->m_audioTriggerParams.m_coneOuterAngle, trigger->m_audioTriggerParams.m_coneOuterGrain);
            ma_sound_set_pitch(sound, trigger->m_audioTriggerParams.m_pitch);
            ma_sound_set_doppler_factor(sound, trigger->m_audioTriggerParams.m_dopplerFactor);
            ma_sound_set_looping(sound, trigger->m_audioTriggerParams.m_looping ? MA_TRUE : MA_FALSE);
            ma_sound_set_volume(sound, trigger->m_audioTriggerParams.m_volume);
            ma_sound_set_min_gain(sound, trigger->m_audioTriggerParams.m_minGains);
            ma_sound_set_max_gain(sound, trigger->m_audioTriggerParams.m_maxGains);

            auto posVec = object->m_position;
            ma_sound_set_position(sound, posVec.GetX(), posVec.GetY(), posVec.GetZ());
        }

        ma_sound_start(sound);
        event->m_isPlayingEvent = true;
        event->m_soundInstance = sound;
        object->m_activeMASounds.emplace(audioFilePath, ActiveMASoundData { false, decoder, sound, trigger->m_audioTriggerParams.m_volume });
        break;
    }
    case SoundAction::Stop: {
        auto range = object->m_activeMASounds.equal_range(audioFilePath);
        for(auto it = range.first; it != range.second; ++it)
        {
            ma_sound_stop(it->second.m_sound);
            ma_sound_uninit(it->second.m_sound);
            azdestroy(it->second.m_sound, Audio::AudioImplAllocator, ma_sound);

            ma_decoder_uninit(it->second.m_decoder);
            azdestroy(it->second.m_decoder, Audio::AudioImplAllocator, ma_decoder);
        }
        object->m_activeMASounds.erase(audioFilePath);
        break;
    }
    case SoundAction::Pause: {
        auto range = object->m_activeMASounds.equal_range(audioFilePath);
        for(auto it = range.first; it != range.second; ++it)
        {
            ma_sound_stop(it->second.m_sound);
            it->second.m_isPaused = true;
        }
        break;
    }
    case SoundAction::Resume: {
        auto range = object->m_activeMASounds.equal_range(audioFilePath);
        for(auto it = range.first; it != range.second; ++it)
        {
            if(!it->second.m_isPaused)
                continue;

            ma_sound_start(it->second.m_sound);
            it->second.m_isPaused = false;
        }
        break;
    }
    default:
        return EAudioRequestStatus::Failure;
    }

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::StopEvent(Audio::IATLAudioObjectData *objectData, const Audio::IATLEventData *eventData)
{
    auto object = static_cast<SATLAudioObjectData_MA*>(objectData);
    auto event  = static_cast<const SATLEventData_MA*>(eventData);

    if(!object || !event)
    {
        return EAudioRequestStatus::Failure;
    }

    if(!event->m_isPlayingEvent)
    {
        return EAudioRequestStatus::Success;
    }

    AZ_Trace(Constants::LogWindow, "Trying to stop event... ");
    ma_sound_stop(event->m_soundInstance);

    for(auto& pair : object->m_activeMASounds)
    {
        if(pair.second.m_sound == event->m_soundInstance)
        {
            AZ_Info(Constants::LogWindow, "Stopping Trigger: %s", pair.first.c_str());
            ma_sound_stop(pair.second.m_sound);
            ma_sound_uninit(pair.second.m_sound);
            azdestroy(pair.second.m_sound, Audio::AudioImplAllocator, ma_sound);

            ma_decoder_uninit(pair.second.m_decoder);
            azdestroy(pair.second.m_decoder);

            object->m_activeMASounds.erase(pair.first);
            break;
        }
    }

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::StopAllEvents(Audio::IATLAudioObjectData *objectData)
{
    auto object = static_cast<SATLAudioObjectData_MA*>(objectData);
    if(!object)
    {
        return EAudioRequestStatus::Failure;
    }

    AZ_Info(Constants::LogWindow, "Stopping all events of objects!");
    for(auto& pair : object->m_activeMASounds)
    {
        ma_sound_stop(pair.second.m_sound);
        ma_sound_uninit(pair.second.m_sound);
        azdestroy(pair.second.m_sound, Audio::AudioImplAllocator, ma_sound);

        ma_decoder_uninit(pair.second.m_decoder);
        azdestroy(pair.second.m_decoder);
    }

    object->m_activeMASounds.clear();
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetPosition(Audio::IATLAudioObjectData *objectData, const Audio::SATLWorldPosition &worldPosition)
{
    auto object = static_cast<SATLAudioObjectData_MA*>(objectData);
    if(!object)
    {
        return EAudioRequestStatus::Failure;
    }

    AZ::Vector3 posVec = worldPosition.GetPositionVec();

    object->m_position = posVec;

    for(auto& pair : object->m_activeMASounds)
    {
        ma_sound_set_position(pair.second.m_sound, posVec.GetX(), posVec.GetY(), posVec.GetZ());
    }

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetMultiplePositions(Audio::IATLAudioObjectData *objectData, const Audio::MultiPositionParams &multiPositions)
{
    return EAudioRequestStatus::Failure;
}

EAudioRequestStatus AudioSystemImpl_MA::SetRtpc(Audio::IATLAudioObjectData *objectData, const Audio::IATLRtpcImplData *rtpcData, float value)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetSwitchState(Audio::IATLAudioObjectData *objectData, const Audio::IATLSwitchStateImplData *switchStateData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetObstructionOcclusion(Audio::IATLAudioObjectData *objectData, float obstruction, float occlusion)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetEnvironment(Audio::IATLAudioObjectData *objectData, const Audio::IATLEnvironmentImplData *environmentData, float amount)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetListenerPosition(Audio::IATLListenerData *listenerData, const Audio::SATLWorldPosition &newPosition)
{
    auto listener = static_cast<SATLListenerData_MA*>(listenerData);
    AZ::Vector3 pos = newPosition.GetPositionVec();
    AZ::Vector3 up  = newPosition.GetUpVec();

    ma_engine_listener_set_position(m_engine.get(), listener->m_listenerIndex, pos.GetX(), pos.GetY(), pos.GetZ());
    ma_engine_listener_set_world_up(m_engine.get(), listener->m_listenerIndex, up.GetX(), up.GetY(), up.GetZ());

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::ResetRtpc(Audio::IATLAudioObjectData *objectData, const Audio::IATLRtpcImplData *rtpcData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::RegisterInMemoryFile(Audio::SATLAudioFileEntryInfo *audioFileEntry)
{
    if(audioFileEntry)
    {
        auto implData = static_cast<SATLAudioFileEntryData_MA*>(audioFileEntry->pImplData);
        if(!implData)
        {
            return EAudioRequestStatus::Failure;
        }

        AudioSourceBuffer audioBuffer(audioFileEntry->pFileData, audioFileEntry->nSize);
        AZ_Info(Constants::LogWindow, "Audio File '%s' loaded into memory!", implData->m_audioFilePath.c_str());
        m_audioSources.emplace(implData->m_audioFilePath, AZStd::move(audioBuffer));
        return EAudioRequestStatus::Success;
    }
    return EAudioRequestStatus::Failure;
}

EAudioRequestStatus AudioSystemImpl_MA::UnregisterInMemoryFile(Audio::SATLAudioFileEntryInfo *audioFileEntry)
{
    auto data = static_cast<const SATLAudioFileEntryData_MA*>(audioFileEntry->pImplData);
    if(!data)
    {
        return EAudioRequestStatus::Failure;
    }

    auto it = m_audioSources.find(data->m_audioFilePath);
    if(it == m_audioSources.end())
    {
        return EAudioRequestStatus::Failure;
    }


    m_audioSources.erase(it);

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::ParseAudioFileEntry(const AZ::rapidxml::xml_node<char> *audioFileEntryNode, Audio::SATLAudioFileEntryInfo *fileEntryInfo)
{
    EAudioRequestStatus result = EAudioRequestStatus::Failure;
    if(audioFileEntryNode && AZ::StringFunc::Equal(audioFileEntryNode->name(), MAXMLTags::AudioFileTag))
    {
        const char* audioFileEntryName = nullptr;
        auto fileEntryNameAttr = audioFileEntryNode->first_attribute(MAXMLTags::AudioFilePathAttr, 0, false);
        if(fileEntryNameAttr)
        {
            audioFileEntryName = fileEntryNameAttr->value();
        }
        bool isLocalized = false;
        auto localizedAttr = audioFileEntryNode->first_attribute(MAXMLTags::AudioFileLocalizedAttr, 0, false);
        if(localizedAttr)
        {
            isLocalized = AZ::StringFunc::Equal(localizedAttr->value(), "true");
        }
        if (audioFileEntryName && audioFileEntryName[0] != '\0')
        {
            fileEntryInfo->bLocalized = isLocalized;
            fileEntryInfo->sFileName = audioFileEntryName;

            auto data = azcreate(SATLAudioFileEntryData_MA, (), Audio::AudioImplAllocator);
            AZ::IO::FixedMaxPath ffPath = audioFileEntryName;
            if(isLocalized)
            {
                ffPath = AZ::IO::FixedMaxPath(Config::LocalizationDirName) / m_currentLanguage / ffPath;
            }
            data->m_audioFilePath = ffPath;
            fileEntryInfo->pImplData = data;
            result = EAudioRequestStatus::Success;
        }
    }
    AZ_Warning(Constants::LogWindow, result == EAudioRequestStatus::Success, "Failure trying to parse a data file entry node!");
    return result;
}

void AudioSystemImpl_MA::DeleteAudioFileEntryData(Audio::IATLAudioFileEntryData *oldAudioFileEntryData)
{
    azdestroy(oldAudioFileEntryData, Audio::AudioImplAllocator, SATLAudioFileEntryData_MA);
}

const char * const AudioSystemImpl_MA::GetAudioFileLocation(Audio::SATLAudioFileEntryInfo *fileEntryInfo)
{
    if(!fileEntryInfo)
    {
        return nullptr;
    }

    if(fileEntryInfo->bLocalized)
    {
        return m_localizationPath.c_str();
    }
    else
    {
        return Config::AudioFilesPath.c_str();
    }
}

IATLTriggerImplData *AudioSystemImpl_MA::NewAudioTriggerImplData(const AZ::rapidxml::xml_node<char> *audioTriggerNode)
{
    if(!audioTriggerNode)
    {
        return nullptr;
    }

    if(!AZ::StringFunc::Equal(audioTriggerNode->name(), MAXMLTags::AudioFileTag))
    {
        return nullptr;
    }

    auto attr = audioTriggerNode->first_attribute(MAXMLTags::AudioFilePathAttr);
    if( !attr || !attr->value() || attr->value()[0] == '\0')
    {
        return nullptr;
    }

    const char* audioFilePath = attr->value();
    bool isLocalized = false;

    attr = audioTriggerNode->first_attribute(MAXMLTags::AudioFileLocalizedAttr);
    if(attr)
    {
        isLocalized = AZ::StringFunc::Equal(attr->value(), "true") ? true : false;
    }
    AZ::IO::FixedMaxPath fullPath = audioFilePath;
    if(isLocalized)
    {
        fullPath = AZ::IO::FixedMaxPath(Config::LocalizationDirName) / m_currentLanguage / fullPath;
    }

    SATLTriggerImplData_MA* triggerImpl =
            azcreate(SATLTriggerImplData_MA, (), Audio::AudioImplAllocator);

    if(!triggerImpl) {
        return nullptr;
    }

    triggerImpl->m_audioFilePath = fullPath;
    triggerImpl->m_audioTriggerParams.ReadFromXML(*audioTriggerNode);
    return triggerImpl;
}

void AudioSystemImpl_MA::DeleteAudioTriggerImplData(Audio::IATLTriggerImplData *oldTriggerData)
{
    azdestroy(oldTriggerData, Audio::AudioImplAllocator, SATLTriggerImplData_MA);
}

IATLRtpcImplData *AudioSystemImpl_MA::NewAudioRtpcImplData(const AZ::rapidxml::xml_node<char> *audioRtpcNode)
{
    return nullptr;
}

void AudioSystemImpl_MA::DeleteAudioRtpcImplData(Audio::IATLRtpcImplData *oldRtpcData)
{

}

IATLSwitchStateImplData *AudioSystemImpl_MA::NewAudioSwitchStateImplData(const AZ::rapidxml::xml_node<char> *audioSwitchStateNode)
{
    return nullptr;
}

void AudioSystemImpl_MA::DeleteAudioSwitchStateImplData(Audio::IATLSwitchStateImplData *oldAudioSwitchStateData)
{

}

IATLEnvironmentImplData *AudioSystemImpl_MA::NewAudioEnvironmentImplData(const AZ::rapidxml::xml_node<char> *audioEnvironmentNode)
{
    return nullptr;
}

void AudioSystemImpl_MA::DeleteAudioEnvironmentImplData(Audio::IATLEnvironmentImplData *oldEnvironmentData)
{

}

IATLAudioObjectData *AudioSystemImpl_MA::NewGlobalAudioObjectData(Audio::TAudioObjectID objectId)
{
    return NewAudioObjectData(objectId);
}

IATLAudioObjectData *AudioSystemImpl_MA::NewAudioObjectData(Audio::TAudioObjectID objectId)
{
    AZ_UNUSED(objectId);
    AudioObjectPtr obj(azcreate(SATLAudioObjectData_MA, (), Audio::AudioImplAllocator));
    if(!obj)
    {
        return nullptr;
    }

    auto pair = m_audioObjects.emplace(AZStd::move(obj));
    if(!pair.second)
    {
        return nullptr;
    }

    return pair.first->get();
}

void AudioSystemImpl_MA::DeleteAudioObjectData(Audio::IATLAudioObjectData *oldObjectData)
{
    if(!oldObjectData)
        return;

    auto pred = [&](AudioObjectPtr& ptr) { return ptr.get() == oldObjectData; };
    auto it = AZStd::find_if(m_audioObjects.begin(), m_audioObjects.end(), pred);
    if (it != m_audioObjects.end())
    {
        m_audioObjects.erase(it);
    }
}

IATLListenerData *AudioSystemImpl_MA::NewDefaultAudioListenerObjectData(Audio::TATLIDType objectId)
{
    //Index 0 is always reserved for this default audio listener, and it's enabled by default.
    auto listener = azcreate(SATLListenerData_MA, (0), Audio::AudioImplAllocator);
    return listener;
}

IATLListenerData *AudioSystemImpl_MA::NewAudioListenerObjectData(Audio::TATLIDType objectId)
{
    if(m_availableListeners.empty())
    {
        AZ_Error(Constants::LogWindow, false, "Error: We reached the amount listeners max supported by MiniAudio!");
        return nullptr;
    }

    int newListenerIndex = m_availableListeners.back();
    m_availableListeners.pop_back();

    ma_engine_listener_set_enabled(m_engine.get(), newListenerIndex, MA_TRUE);
    auto listener = azcreate(SATLListenerData_MA, (newListenerIndex), Audio::AudioImplAllocator);
    return listener;
}

void AudioSystemImpl_MA::DeleteAudioListenerObjectData(Audio::IATLListenerData *oldListenerData)
{
    auto listener = static_cast<SATLListenerData_MA*>(oldListenerData);
    if(listener->m_listenerIndex != 0)
    {
        ma_engine_listener_set_enabled(m_engine.get(), listener->m_listenerIndex, MA_FALSE);
        m_availableListeners.push_back(listener->m_listenerIndex);
    }

    azdestroy(oldListenerData, Audio::AudioImplAllocator, Audio::IATLListenerData);
}

IATLEventData *AudioSystemImpl_MA::NewAudioEventData(Audio::TAudioEventID eventID)
{
    return azcreate(SATLEventData_MA, (), Audio::AudioImplAllocator);
}

void AudioSystemImpl_MA::DeleteAudioEventData(Audio::IATLEventData *oldEventData)
{
    azdestroy(oldEventData, Audio::AudioImplAllocator, Audio::IATLEventData);
}

void AudioSystemImpl_MA::ResetAudioEventData(Audio::IATLEventData *eventData)
{
    auto event = static_cast<SATLEventData_MA*>(eventData);
    if(event)
    {
        *event = SATLEventData_MA();
    }
}

void AudioSystemImpl_MA::SetLanguage(const char *language)
{
    if(!language)
    {
        return;
    }

    m_currentLanguage = language;
    AZStd::to_lower(m_currentLanguage.begin(), m_currentLanguage.end());
    m_localizationPath = Config::AudioFilesPath / Config::LocalizationDirName / m_currentLanguage;

}

const char * const AudioSystemImpl_MA::GetImplSubPath() const
{
    return "miniaudio/";
}

const char * const AudioSystemImpl_MA::GetImplementationNameString() const
{
    return "MiniAudio";
}

void AudioSystemImpl_MA::GetMemoryInfo(Audio::SAudioImplMemoryInfo &memoryInfo) const
{
    memoryInfo.nPrimaryPoolSize = AZ::AllocatorInstance<Audio::AudioImplAllocator>::Get().Capacity();
    memoryInfo.nPrimaryPoolUsedSize =
        memoryInfo.nPrimaryPoolSize - AZ::AllocatorInstance<Audio::AudioImplAllocator>::Get().NumAllocatedBytes();
    memoryInfo.nPrimaryPoolAllocations = 0;
    memoryInfo.nSecondaryPoolSize = 0;
    memoryInfo.nSecondaryPoolUsedSize = 0;
    memoryInfo.nSecondaryPoolAllocations = 0;
}

AZStd::vector<AudioImplMemoryPoolInfo> AudioSystemImpl_MA::GetMemoryPoolInfo()
{
    return AZStd::vector<AudioImplMemoryPoolInfo>();
}

bool AudioSystemImpl_MA::CreateAudioSource(const Audio::SAudioInputConfig &sourceConfig)
{
    return false;
}

void AudioSystemImpl_MA::DestroyAudioSource(Audio::TAudioSourceId sourceId)
{

}

void AudioSystemImpl_MA::SetPanningMode(Audio::PanningMode mode)
{
    //Not available in MiniAudio.
}

void AudioSystemImpl_MA::OnAudioSystemLoseFocus()
{
    ma_engine_stop(m_engine.get());
}

void AudioSystemImpl_MA::OnAudioSystemGetFocus()
{
    ma_engine_start(m_engine.get());
}

void AudioSystemImpl_MA::OnAudioSystemMuteAll()
{
    m_globalVolume = ma_engine_get_volume(m_engine.get());
    ma_engine_set_volume(m_engine.get(), 0.0);
}

void AudioSystemImpl_MA::OnAudioSystemUnmuteAll()
{
    ma_engine_set_volume(m_engine.get(), m_globalVolume);
}

void AudioSystemImpl_MA::OnAudioSystemRefresh()
{
    m_audioSources.clear();
}

void AudioSystemImpl_MA::CheckObjectForExpiredMASounds(SATLAudioObjectData_MA &audioObj)
{
    AZStd::vector<decltype(audioObj.m_activeMASounds)::iterator> iterators;
    iterators.reserve(audioObj.m_activeMASounds.size());

    for(auto it = audioObj.m_activeMASounds.begin(); it != audioObj.m_activeMASounds.end(); ++it)
    {
        if(it->second.m_isPaused)
            continue;

        bool isAtEnd   = ma_sound_at_end(it->second.m_sound) == MA_TRUE;

        if(isAtEnd)
        {
            iterators.push_back(it);
        }
    }

    if(!iterators.empty())
        AZ_Info(Constants::LogWindow, "Found %d expired ma_sounds ready for deletion/cleanup", iterators.size());

    for(auto it : iterators)
    {
        ma_sound_uninit(it->second.m_sound);
        azdestroy(it->second.m_sound, Audio::AudioImplAllocator, ma_sound);

        ma_decoder_uninit(it->second.m_decoder);
        azdestroy(it->second.m_decoder, Audio::AudioImplAllocator, ma_decoder);

        audioObj.m_activeMASounds.erase(it);
    }
}

}