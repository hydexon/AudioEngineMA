
#include "AudioEngineImpl_MA.h"
#include "Common_MA.h"
#include "Config_MA.h"
#include "FileIOVFS_MA.h"

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
    engineCfg.channels = 0;

    if(ma_result eng_res = ma_engine_init(&engineCfg, m_engine.get());
            eng_res != MA_SUCCESS)
    {
        ma_resource_manager_uninit(m_resourceManager.get());
        m_resourceManager.reset();
        m_engine.reset();

        AZ_Error(Constants::LogWindow, false, "Failure initializing MiniAudio: %d", eng_res);
        return EAudioRequestStatus::Failure;
    }

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
    auto object   = static_cast<SATLAudioObjectData_MA*>(objectData);
    auto trigger  = static_cast<const SATLTriggerImplData_MA*>(triggerData);
    auto event    = static_cast<SATLEventData_MA*>(eventData);

    if(!object || !trigger || !event)
    {
        return EAudioRequestStatus::Failure;
    }

    auto audioSrcIt = m_audioSources.find(trigger->m_audioFilePath);
    if(audioSrcIt == m_audioSources.end()) {
        return EAudioRequestStatus::Failure;
    }

    ma_result mares = ma_sound_init_from_data_source(m_engine.get(), audioSrcIt->second.get(), 0, NULL, event->sndInstance);
    if(mares != MA_SUCCESS) {
        return EAudioRequestStatus::Failure;
    }

    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::StopEvent(Audio::IATLAudioObjectData *objectData, const Audio::IATLEventData *eventData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::StopAllEvents(Audio::IATLAudioObjectData *objectData)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetPosition(Audio::IATLAudioObjectData *objectData, const Audio::SATLWorldPosition &worldPosition)
{
    return EAudioRequestStatus::Success;
}

EAudioRequestStatus AudioSystemImpl_MA::SetMultiplePositions(Audio::IATLAudioObjectData *objectData, const Audio::MultiPositionParams &multiPositions)
{
    return EAudioRequestStatus::Success;
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
        if(implData)
        {
            return EAudioRequestStatus::Failure;
        }

        AudioSourcePtr audioSource(azcreate(ma_decoder,(), Audio::AudioImplAllocator));
        if(!audioSource)
        {
            return EAudioRequestStatus::Failure;
        }

        ma_result result = ma_decoder_init_memory(audioFileEntry->pFileData, audioFileEntry->nSize, nullptr, audioSource.get());
        if(result != MA_SUCCESS)
        {
            return EAudioRequestStatus::Failure;
        }

        m_audioSources.emplace(implData->m_audioFilePath, AZStd::move(audioSource));
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
    ma_decoder_uninit(it->second.get());
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
    }}

IATLTriggerImplData *AudioSystemImpl_MA::NewAudioTriggerImplData(const AZ::rapidxml::xml_node<char> *audioTriggerNode)
{
    return nullptr;
}

void AudioSystemImpl_MA::DeleteAudioTriggerImplData(Audio::IATLTriggerImplData *oldTriggerData)
{

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
    return nullptr;
}

IATLListenerData *AudioSystemImpl_MA::NewAudioListenerObjectData(Audio::TATLIDType objectId)
{
    return nullptr;
}

void AudioSystemImpl_MA::DeleteAudioListenerObjectData(Audio::IATLListenerData *oldListenerData)
{

}

IATLEventData *AudioSystemImpl_MA::NewAudioEventData(Audio::TAudioEventID eventID)
{
    return nullptr;
}

void AudioSystemImpl_MA::DeleteAudioEventData(Audio::IATLEventData *oldEventData)
{

}

void AudioSystemImpl_MA::ResetAudioEventData(Audio::IATLEventData *eventData)
{

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

}

void AudioSystemImpl_MA::OnAudioSystemLoseFocus()
{

}

void AudioSystemImpl_MA::OnAudioSystemGetFocus()
{

}

void AudioSystemImpl_MA::OnAudioSystemMuteAll()
{

}

void AudioSystemImpl_MA::OnAudioSystemUnmuteAll()
{

}

void AudioSystemImpl_MA::OnAudioSystemRefresh()
{

}

void AudioSystemImpl_MA::CheckObjectForExpiredMASounds(SATLAudioObjectData_MA &audioObj)
{
    AZStd::vector<decltype(audioObj.m_activeMASounds)::iterator> iterators;
    iterators.reserve(audioObj.m_activeMASounds.size());

    for(auto it = audioObj.m_activeMASounds.begin(); it != audioObj.m_activeMASounds.end(); ++it)
    {
        if(!ma_sound_is_playing(it->second.m_sound))
        {
            iterators.push_back(it);
        }
    }

    for(auto it : iterators)
    {
        ma_sound_uninit(it->second.m_sound);
        audioObj.m_activeMASounds.erase(it);
    }
}

}