
#pragma once

#include <AudioEngineMA/AudioEngineMATypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>

namespace AudioEngineMA
{
    class AudioEngineMARequests
    {
    public:
        AZ_RTTI(AudioEngineMARequests, AudioEngineMARequestsTypeId);
        virtual ~AudioEngineMARequests() = default;
        // Put your public methods here
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

        virtual void LoadSoundGroupDefinitions(AZStd::string defFilePath) = 0;
    };

    using AudioEngineMARequestBus = AZ::EBus<AudioEngineMARequests, AudioEngineMABusTraits>;
    using AudioEngineMAInterface = AZ::Interface<AudioEngineMARequests>;

} // namespace AudioEngineMA
