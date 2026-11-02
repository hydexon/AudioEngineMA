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

        virtual void ResetSoundGroupLayout() = 0;
        virtual void LoadSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) = 0;
        virtual void SaveSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) = 0;

        virtual void AddSoundGroup(AZ::Name name, uint32_t flags) = 0;
        virtual void RemoveSoundGroup(AZ::Name name) = 0;
        virtual void ChangeSoundGroupName(AZ::Name name) = 0;

        virtual void AssignSoundToSoundGroup(AZ::Name name, ma_sound* sound) = 0;
        virtual void RemoveSoundFromSoundGroup(AZ::Name name, ma_sound* sound) = 0;

        virtual void SetSoundGroupVolume(AZ::Name name, float volume) = 0;
        virtual void SetSoundGroupPitch(AZ::Name name, float pitch) = 0;
        virtual void SetSoundGroupPan(AZ::Name name, float pan) = 0;
        virtual void SetSoundGroupAttenuationMode(AZ::Name name, AttenuationMode::Type type) = 0;
        virtual void SetSoundGroupPositioning(AZ::Name name, PositioningMode::Type type) = 0;
        virtual void SetSoundGroupPanMode(AZ::Name name, PanMode::Type type) = 0;
        virtual void SetDopplerFactor(AZ::Name name,float factor) = 0;
        virtual void SetMinDistances(AZ::Name name, float minDistance) = 0;
        virtual void SetMaxDistances(AZ::Name name, float maxDistance) = 0;
        virtual void SetMinGains(AZ::Name name, float minGain) = 0;
        virtual void SetMaxGains(AZ::Name name, float maxGain) = 0;
        virtual void SetAttenuationConeInnerAngle(AZ::Name name, float angle) = 0;
        virtual void SetAttenuationConeOuterAngle(AZ::Name name, float angle) = 0;
        virtual void SetAttenuationConeOuterGrain(AZ::Name name, float angle) = 0;

        virtual void RequestSoundGroupLayout() = 0;
        virtual void RequestSoundGroupNames() = 0;
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

        virtual void OnRequestFinished_AddSoundGroup([[maybe_unused]]bool success, [[maybe_unused]] SoundGroupData data) {};
        virtual void OnRequestFinished_RemoveSoundGroup([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name soundGroupName) {};
        virtual void OnRequestFinished_RenameSoundGroup([[maybe_unused]]bool success, [[maybe_unused]] AZ::Name newGroupName) {};

    };
    using SoundGroupManagerNotificationBus = AZ::EBus<SoundGroupManagerNotifications>;

}