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

        virtual void RequestSoundGroupLayout() = 0;
        virtual void ResetSoundGroupLayout() = 0;
        virtual void LoadSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) = 0;
        virtual void SaveSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) = 0;

        virtual void AddSoundGroup(AZ::Name name, AZ::Name parentName = AZ::Name()) = 0;
        virtual void RemoveSoundGroup(AZ::Name name) = 0;
        virtual void ChangeSoundGroupName(AZ::Name oldname, AZ::Name newName) = 0;

        virtual void SetSoundGroupVolume(AZ::Name name, float volume) = 0;
        virtual void SetSoundGroupPitch(AZ::Name name, float pitch) = 0;
        virtual void SetSoundGroupPan(AZ::Name name, float pan) = 0;
        virtual void SetSoundGroupRolloff(AZ::Name name, float rolloff) = 0;
        virtual void SetSoundGroupMinDistance(AZ::Name name, float minDistance) = 0;
        virtual void SetSoundGroupMaxDistance(AZ::Name name, float maxDistance) = 0;
        virtual void SetSoundGroupMinGains(AZ::Name name, float minGains) = 0;
        virtual void SetSoundGroupMaxGains(AZ::Name name, float maxGains) = 0;
        virtual void SetSoundGroupAttenuationMode(AZ::Name name, AttenuationMode::Type type) = 0;
        virtual void SetSoundGroupPositioning(AZ::Name name, PositioningMode::Type type) = 0;
        virtual void SetSoundGroupPanMode(AZ::Name name, PanMode::Type type) = 0;
        virtual void SetSoundGroupDopplerFactor(AZ::Name name,float factor) = 0;
        virtual void SetSoundGroupAttenuationCone(AZ::Name name, float innerAngle, float outerAngle, float outerGrain) = 0;
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

        virtual void OnRequestCompleted_SetSpatializationEnabled([[maybe_unused]] bool success,[[maybe_unused]]AZ::Name groupName, [[maybe_unused]]bool enabled) {};
        virtual void OnRequestCompleted_SetSoundGroupVolume([[maybe_unused]] bool success, [[maybe_unused]]AZ::Name groupName, [[maybe_unused]]float volume) {};
        virtual void OnRequestCompleted_SetSoundGroupPitch([[maybe_unused]] bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float pitch) {};
        virtual void OnRequestCompleted_SetSoundGroupPan([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float pan) {};
        virtual void OnRequestCompleted_SetSoundGroupRolloff([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float rolloff) {};
        virtual void OnRequestCompleted_SetSoundGroupAttenuationMode([[maybe_unused]] bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]AttenuationMode::Type type) {};
        virtual void OnRequestCompleted_SetSoundGroupPositioning([[maybe_unused]] bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]] PositioningMode::Type type) {};
        virtual void OnRequestCompleted_SetSoundGroupPanMode([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]] PanMode::Type type) {};
        virtual void OnRequestCompleted_SetSoundGroupDopplerFactor([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float factor) {};
        virtual void OnRequestCompleted_SetSoundGroupMinDistance([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float minDistance) {};
        virtual void OnRequestCompleted_SetSoundGroupMaxDistance([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name groupName, [[maybe_unused]]float maxDistance ) {};
        virtual void OnRequestCompleted_SetSoundGroupMinGains([[maybe_unused]]bool success, [[maybe_unused]]AZ::Name groupName, [[maybe_unused]]float minGains) {};
        virtual void OnRequestCompleted_SetSoundGroupMaxGains([[maybe_unused]]bool success, [[maybe_unused]]AZ::Name groupName, [[maybe_unused]]float maxGains) {};
        virtual void OnRequestCompleted_SetSoundGroupAttenuationCone([[maybe_unused]]bool success, [[maybe_unused]]AZ::Name groupName, [[maybe_unused]]float innerAngle, [[maybe_unused]]float outerAngle, [[maybe_unused]]float outerGrain) {};

#if 0
#endif
        virtual void OnUpdateSoundGroupLayout([[maybe_unused]] SoundGroupLayoutData sndGroupLayout) {};

    };
    using SoundGroupManagerNotificationBus = AZ::EBus<SoundGroupManagerNotifications>;

}