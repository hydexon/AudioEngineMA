#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/Name/Name.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

#include <Clients/SoundGroupData.h>
#include <Clients/ATLData.h>
#include <miniaudio.h>

namespace AudioEngineMA
{
    class SoundGroupManagerRequests : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        using MutexType = AZStd::mutex;
        static const bool EnableEventQueue = true;

        virtual ~SoundGroupManagerRequests() = default;
#if 0
        virtual void ResetSoundGroupLayout() = 0;
        virtual void LoadSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) = 0;
        virtual void SaveSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) = 0;
#endif
        virtual void AddSoundGroup(AZ::Name name, AZ::Name parentName = AZ::Name()) = 0;
        virtual void RemoveSoundGroup(AZ::Name name) = 0;
        virtual void ChangeSoundGroupName(AZ::Name oldname, AZ::Name newName) = 0;
#if 0
        virtual void AssignSoundToSoundGroup(AZ::Name name, ma_sound* sound) = 0;
        virtual void RemoveSoundFromSoundGroup(AZ::Name name, ma_sound* sound) = 0;

        virtual void SetSoundGroupVolume(AZ::Name name, float volume) = 0;
        virtual void SetSoundGroupPitch(AZ::Name name, float pitch) = 0;
        virtual void SetSoundGroupPan(AZ::Name name, float pan) = 0;
        virtual void SetSoundGroupAttenuationMode(AZ::Name name, AttenuationMode::Type type) = 0;
        virtual void SetSoundGroupPositioning(AZ::Name name, PositioningMode::Type type) = 0;
        virtual void SetSoundGroupPanMode(AZ::Name name, PanMode::Type type) = 0;
        virtual void SetSoundGroupDopplerFactor(AZ::Name name,float factor) = 0;
        virtual void SetSoundGroupDistances(AZ::Name name, float minDistance, float maxDistance) = 0;
        virtual void SetSoundGroupGains(AZ::Name name, float minGain, float maxGain) = 0;
        virtual void SetSoundGroupAttenuationCone(AZ::Name name, float innerAngle, float outerAngle, float outerGrain) = 0;

        virtual void RequestSoundGroupLayout() = 0;
        virtual void RequestSoundGroupNames() = 0;
#endif
    };
    using SoundGroupManagerRequestBus = AZ::EBus<SoundGroupManagerRequests>;

    class SoundGroupManagerNotifications : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        using MutexType = AZStd::recursive_mutex;
        static const bool EnableEventQueue = true;

        virtual ~SoundGroupManagerNotifications() = default;

        virtual void OnRequestCompleted_AddSoundGroup([[maybe_unused]]bool success, [[maybe_unused]] SoundGroupData data, [[maybe_unused]] AZ::Name parentGroup) {};
        virtual void OnRequestCompleted_RemoveSoundGroup([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name soundGroupName) {};
        virtual void OnRequestCompleted_RenameSoundGroup([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name oldGroupName, [[maybe_unused]] AZ::Name newGroupName) {};
#if 0
        virtual void OnRequestCompleted_SetSoundGroupVolume([[maybe_unused]] bool success, [[maybe_unused]]AZ::Name groupName, [[maybe_unused]]float volume) {};
        virtual void OnRequestCompleted_SetSoundGroupPitch([[maybe_unused]] bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float pitch) {};
        virtual void OnRequestCompleted_SetSoundGroupPan([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float pan) {};
        virtual void OnRequestCompleted_SetSoundGroupAttenuationMode([[maybe_unused]] bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]AttenuationMode::Type type) {};
        virtual void OnRequestCompleted_SetSoundGroupPositioning([[maybe_unused]] bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]] PositioningMode::Type type) {};
        virtual void OnRequestCompleted_SetSoundGroupPanMode([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]] PanMode::Type type) {};
        virtual void OnRequestCompleted_SetSoundGroupDopplerFactor([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float factor) {};
        virtual void OnRequestCompleted_SetSoundGroupDistances([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float minDistance, [[maybe_unused]] float maxDistance ) {};
        virtual void OnRequestCompleted_SetSoundGroupGains([[maybe_unused]]bool success, [[maybe_unused]]AZ::Name groupName, [[maybe_unused]]float minGains, [[maybe_unused]]float maxGains) {};
        virtual void OnRequestCompleted_SetSoundGroupAttenuationCone([[maybe_unused]]bool success, [[maybe_unused]]AZ::Name groupName, [[maybe_unused]]float innerAngle, [[maybe_unused]]float outerAngle, [[maybe_unused]]float outerGrain) {};

        virtual void OnUpdateSoundGroupLayout([[maybe_unused]] SoundGroupLayoutData sndGroupLayout) {};
        virtual void OnUpdateSoundGroupNames([[maybe_unused]] AZStd::vector<AZ::Name> groupNames) {};
#endif

    };
    using SoundGroupManagerNotificationBus = AZ::EBus<SoundGroupManagerNotifications>;

}