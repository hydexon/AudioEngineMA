
#pragma once

#include <AudioEngineMA/AudioEngineMATypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>
#include "Clients/ATLData.h"

namespace AudioEngineMA
{
    class AudioEngineMARequests
    {
    public:
        AZ_RTTI(AudioEngineMARequests, AudioEngineMARequestsTypeId);
        virtual ~AudioEngineMARequests() = default;

        virtual void LoadSoundGroupLayout(AZStd::string defFilePath) = 0;
        virtual void SetSoundGroupSpatializationEnabled(AZStd::string groupName, bool value) = 0;
        virtual void SetSoundGroupVolume(AZStd::string groupName, float volume) = 0;
        virtual void SetSoundGroupPitch(AZStd::string groupName, float pitch) = 0;
        virtual void SetSoundGroupPan(AZStd::string groupName, float pan) = 0;
        virtual void SetSoundGroupRolloff(AZStd::string groupName, float rolloff) = 0;
        virtual void SetSoundGroupMinDistance(AZStd::string groupName, float minDistance) = 0;
        virtual void SetSoundGroupMaxDistance(AZStd::string groupName, float maxDistance) = 0;
        virtual void SetSoundGroupMinGains(AZStd::string groupName, float minGains) = 0;
        virtual void SetSoundGroupMaxGains(AZStd::string groupName, float maxGains) = 0;
        virtual void SetSoundGroupAttenuationMode(AZStd::string groupName, AttenuationMode::Type type) = 0;
        virtual void SetSoundGroupPositioning(AZStd::string groupName, PositioningMode::Type type) = 0;
        virtual void SetSoundGroupPanMode(AZStd::string groupName, PanMode::Type type) = 0;
        virtual void SetSoundGroupDopplerFactor(AZStd::string groupName,float factor) = 0;
        virtual void SetSoundGroupAttenuationCone(AZStd::string groupName, float innerAngle, float outerAngle, float outerGrain) = 0;

    };

    class AudioEngineMABusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////

        using MutexType = AZStd::mutex;
        static const bool EnableEventQueue = true;

        virtual ~AudioEngineMABusTraits() = default;
    };

    using AudioEngineMARequestBus = AZ::EBus<AudioEngineMARequests, AudioEngineMABusTraits>;
    using AudioEngineMAInterface = AZ::Interface<AudioEngineMARequests>;

} // namespace AudioEngineMA
