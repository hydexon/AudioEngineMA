#pragma once

#include <AudioAllocators.h>
#include <IAudioSystemImplementation.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/IO/Path/Path.h>

#include "ATLEntities_MA.h"
#include <miniaudio.h>

namespace AudioEngineMA
{
    class AudioSystemImpl_MA :
            public Audio::AudioSystemImplementation
    {
    public:
        AUDIO_IMPL_CLASS_ALLOCATOR(AudioSystemImpl_MA);

        AudioSystemImpl_MA();
        ~AudioSystemImpl_MA() override;

        void Update(float updateIntervalMS) override;

        Audio::EAudioRequestStatus Initialize() override;

        Audio::EAudioRequestStatus ShutDown() override;

        Audio::EAudioRequestStatus Release() override;

        Audio::EAudioRequestStatus StopAllSounds() override;

        Audio::EAudioRequestStatus
        RegisterAudioObject(Audio::IATLAudioObjectData *objectData,
                            const char *objectName = nullptr) override;

        Audio::EAudioRequestStatus
        UnregisterAudioObject(Audio::IATLAudioObjectData *objectData) override;

        Audio::EAudioRequestStatus
        ResetAudioObject(Audio::IATLAudioObjectData *objectData) override;

        Audio::EAudioRequestStatus
        UpdateAudioObject(Audio::IATLAudioObjectData *objectData) override;

        Audio::EAudioRequestStatus
        PrepareTriggerSync(Audio::IATLAudioObjectData *audioObjectData,
                           const Audio::IATLTriggerImplData *triggerData) override;

        Audio::EAudioRequestStatus
        UnprepareTriggerSync(Audio::IATLAudioObjectData *objectData,
                             const Audio::IATLTriggerImplData *triggerData) override;

        Audio::EAudioRequestStatus
        PrepareTriggerAsync(Audio::IATLAudioObjectData *objectData,
                            const Audio::IATLTriggerImplData *triggerData,
                            Audio::IATLEventData *eventData) override;

        Audio::EAudioRequestStatus
        UnprepareTriggerAsync(Audio::IATLAudioObjectData *pAudioObjectData,
                              const Audio::IATLTriggerImplData *pTriggerData,
                              Audio::IATLEventData *pEventData) override;

        Audio::EAudioRequestStatus
        ActivateTrigger(Audio::IATLAudioObjectData *objectData,
                        const Audio::IATLTriggerImplData *triggerData,
                        Audio::IATLEventData *eventData,
                        const Audio::SATLSourceData *sourceData) override;

        Audio::EAudioRequestStatus StopEvent(Audio::IATLAudioObjectData *objectData,
                                      const Audio::IATLEventData *eventData) override;

        Audio::EAudioRequestStatus
        StopAllEvents(Audio::IATLAudioObjectData *objectData) override;

        Audio::EAudioRequestStatus
        SetPosition(Audio::IATLAudioObjectData *objectData,
                    const Audio::SATLWorldPosition &worldPosition) override;

        Audio::EAudioRequestStatus
        SetMultiplePositions(Audio::IATLAudioObjectData *objectData,
                             const Audio::MultiPositionParams &multiPositions) override;

        Audio::EAudioRequestStatus SetRtpc(Audio::IATLAudioObjectData *objectData,
                                    const Audio::IATLRtpcImplData *rtpcData,
                                    float value) override;

        Audio::EAudioRequestStatus
        SetSwitchState(Audio::IATLAudioObjectData *objectData,
                       const Audio::IATLSwitchStateImplData *switchStateData) override;

        Audio::EAudioRequestStatus
        SetObstructionOcclusion(Audio::IATLAudioObjectData *objectData,
                                float obstruction, float occlusion) override;

        Audio::EAudioRequestStatus
        SetEnvironment(Audio::IATLAudioObjectData *objectData,
                       const Audio::IATLEnvironmentImplData *environmentData,
                       float amount) override;

        Audio::EAudioRequestStatus
        SetListenerPosition(Audio::IATLListenerData *listenerData,
                            const Audio::SATLWorldPosition &newPosition) override;

        Audio::EAudioRequestStatus ResetRtpc(Audio::IATLAudioObjectData *objectData,
                                      const Audio::IATLRtpcImplData *rtpcData) override;

        Audio::EAudioRequestStatus
        RegisterInMemoryFile(Audio::SATLAudioFileEntryInfo *audioFileEntry) override;

        Audio::EAudioRequestStatus
        UnregisterInMemoryFile(Audio::SATLAudioFileEntryInfo *audioFileEntry) override;

        Audio::EAudioRequestStatus ParseAudioFileEntry(
                const AZ::rapidxml::xml_node<char> *audioFileEntryNode,
                Audio::SATLAudioFileEntryInfo *fileEntryInfo) override;

        void DeleteAudioFileEntryData(
                Audio::IATLAudioFileEntryData *oldAudioFileEntryData) override;

        const char *const
        GetAudioFileLocation(Audio::SATLAudioFileEntryInfo *fileEntryInfo) override;

        Audio::IATLTriggerImplData *NewAudioTriggerImplData(
                const AZ::rapidxml::xml_node<char> *audioTriggerNode) override;

        void
        DeleteAudioTriggerImplData(Audio::IATLTriggerImplData *oldTriggerData) override;

        Audio::IATLRtpcImplData *NewAudioRtpcImplData(
                const AZ::rapidxml::xml_node<char> *audioRtpcNode) override;

        void DeleteAudioRtpcImplData(Audio::IATLRtpcImplData *oldRtpcData) override;

        Audio::IATLSwitchStateImplData *NewAudioSwitchStateImplData(
                const AZ::rapidxml::xml_node<char> *audioSwitchStateNode) override;

        void DeleteAudioSwitchStateImplData(
                Audio::IATLSwitchStateImplData *oldAudioSwitchStateData) override;

        Audio::IATLEnvironmentImplData *NewAudioEnvironmentImplData(
                const AZ::rapidxml::xml_node<char> *audioEnvironmentNode) override;

        void DeleteAudioEnvironmentImplData(
                Audio::IATLEnvironmentImplData *oldEnvironmentData) override;

        Audio::IATLAudioObjectData *
        NewGlobalAudioObjectData(Audio::TAudioObjectID objectId) override;

        Audio::IATLAudioObjectData *
        NewAudioObjectData(Audio::TAudioObjectID objectId) override;

        void DeleteAudioObjectData(Audio::IATLAudioObjectData *oldObjectData) override;

        Audio::IATLListenerData *
        NewDefaultAudioListenerObjectData(Audio::TATLIDType objectId) override;

        Audio::IATLListenerData *
        NewAudioListenerObjectData(Audio::TATLIDType objectId) override;

        void DeleteAudioListenerObjectData(
                Audio::IATLListenerData *oldListenerData) override;

        Audio::IATLEventData *NewAudioEventData(Audio::TAudioEventID eventID) override;

        void DeleteAudioEventData(Audio::IATLEventData *oldEventData) override;

        void ResetAudioEventData(Audio::IATLEventData *eventData) override;

        void SetLanguage(const char *language) override;

        const char *const GetImplSubPath() const override;

        const char *const GetImplementationNameString() const override;

        void GetMemoryInfo(Audio::SAudioImplMemoryInfo &memoryInfo) const override;

        AZStd::vector<Audio::AudioImplMemoryPoolInfo> GetMemoryPoolInfo() override;

        bool CreateAudioSource(const Audio::SAudioInputConfig &sourceConfig) override;

        void DestroyAudioSource(Audio::TAudioSourceId sourceId) override;

        void SetPanningMode(Audio::PanningMode mode) override;

        void OnAudioSystemLoseFocus() override;

        void OnAudioSystemGetFocus() override;

        void OnAudioSystemMuteAll() override;

        void OnAudioSystemUnmuteAll() override;

        void OnAudioSystemRefresh() override;
    private:
        template<class U>
        struct AudioDeleter
        {
          void operator()(U* ptr)
          {
              azdestroy(ptr, Audio::AudioImplAllocator, U);
          }
        };

        using AudioObjectPtr = AZStd::unique_ptr<SATLAudioObjectData_MA, AudioDeleter<SATLAudioObjectData_MA>>;
        using AudioSourcePtr = AZStd::unique_ptr<ma_resource_manager_data_source, AudioDeleter<ma_resource_manager_data_source>>;

        std::unique_ptr<ma_log> m_log;
        std::unique_ptr<ma_engine> m_engine;
        std::unique_ptr<ma_resource_manager> m_resourceManager;

        AZStd::unordered_map<AZ::IO::FixedMaxPath, AudioSourcePtr> m_audioSources;
        AZStd::unordered_set<AudioObjectPtr> m_audioObjects;

        AZStd::string m_currentLanguage;
        AZ::IO::FixedMaxPath m_localizationPath;

        //Listeners:
        AZStd::unordered_map<int, Audio::TATLIDType> m_activeListeners;
        void CheckObjectForExpiredMASounds(SATLAudioObjectData_MA& audioObj);

    };

}