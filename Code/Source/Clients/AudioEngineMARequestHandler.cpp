#include "AudioEngineMARequestHandler.h"
#include "EditorEngineInterop.h"
#include "AzCore/RTTI/BehaviorContext.h"
#include <AzCore/Serialization/EditContext.h>

namespace AudioEngineMA
{
    AudioEngineMARequestHandler::AudioEngineMARequestHandler()
    {
        AudioEngineMARequestBus::Handler::BusConnect();
    }

    AudioEngineMARequestHandler::~AudioEngineMARequestHandler()
    {
        AudioEngineMARequestBus::Handler::BusDisconnect();
    }

    void AudioEngineMARequestHandler::Reflect(AZ::ReflectContext* context)
    {
        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<AudioEngineMARequestBus>("SoundGroupMARequestBus")
                ->Event("LoadSoundGroupLayout", &AudioEngineMARequestBus::Events::LoadSoundGroupLayout)
                ->Event("SetSoundGroupSpatializationEnabled", &AudioEngineMARequestBus::Events::SetSoundGroupSpatializationEnabled)
                ->Event("SetSoundGroupVolume", &AudioEngineMARequestBus::Events::SetSoundGroupVolume)
                ->Event("SetSoundGroupPitch", &AudioEngineMARequestBus::Events::SetSoundGroupPitch)
                ->Event("SetSoundGroupPan", &AudioEngineMARequestBus::Events::SetSoundGroupPan)
                ->Event("SetSoundGroupRolloff", &AudioEngineMARequestBus::Events::SetSoundGroupRolloff)
                ->Event("SetSoundGroupMinDistance", &AudioEngineMARequestBus::Events::SetSoundGroupMinDistance)
                ->Event("SetSoundGroupMaxDistance", &AudioEngineMARequestBus::Events::SetSoundGroupMaxDistance)
                ->Event("SetSoundGroupMinGains", &AudioEngineMARequestBus::Events::SetSoundGroupMinGains)
                ->Event("SetSoundGroupMaxGains", &AudioEngineMARequestBus::Events::SetSoundGroupMaxGains)
                ->Event("SetSoundGroupAttenuationMode", &AudioEngineMARequestBus::Events::SetSoundGroupAttenuationMode)
                ->Event("SetSoundGroupPositioning", &AudioEngineMARequestBus::Events::SetSoundGroupPositioning)
                ->Event("SetSoundGroupPanMode", &AudioEngineMARequestBus::Events::SetSoundGroupPanMode)
                ->Event("SetSoundGroupDopplerFactor", &AudioEngineMARequestBus::Events::SetSoundGroupDopplerFactor)
                ->Event("SetSoundGroupAttenuationCone", &AudioEngineMARequestBus::Events::SetSoundGroupAttenuationCone);
        }
    }

    void AudioEngineMARequestHandler::LoadSoundGroupLayout(AZStd::string defFilePath)
    {
        AZ::IO::FixedMaxPath path = defFilePath.c_str();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::LoadSoundGroupLayout, path);
    }

    void AudioEngineMARequestHandler::SetSoundGroupSpatializationEnabled(AZStd::string groupName, bool value)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupSpatializationEnabled, name, value);
    }

    void AudioEngineMARequestHandler::SetSoundGroupVolume(AZStd::string groupName, float volume)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupVolume, name, volume);
    }

    void AudioEngineMARequestHandler::SetSoundGroupPitch(AZStd::string groupName, float pitch)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPitch, name, pitch);

    }

    void AudioEngineMARequestHandler::SetSoundGroupPan(AZStd::string groupName, float pan)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPan, name, pan);
    }

    void AudioEngineMARequestHandler::SetSoundGroupRolloff(AZStd::string groupName, float rolloff)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupRolloff, name, rolloff);
    }

    void AudioEngineMARequestHandler::SetSoundGroupMinDistance(AZStd::string groupName, float minDistance)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMinDistance, name, minDistance);
    }

    void AudioEngineMARequestHandler::SetSoundGroupMaxDistance(AZStd::string groupName, float maxDistance)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMaxDistance, name, maxDistance);
    }

    void AudioEngineMARequestHandler::SetSoundGroupMinGains(AZStd::string groupName, float minGains)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMinGains, name, minGains);
    }

    void AudioEngineMARequestHandler::SetSoundGroupMaxGains(AZStd::string groupName, float maxGains)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMaxGains, name, maxGains);
    }

    void AudioEngineMARequestHandler::SetSoundGroupAttenuationMode(AZStd::string groupName, AttenuationMode::Type type)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupAttenuationMode, name, type);
    }

    void AudioEngineMARequestHandler::SetSoundGroupPositioning(AZStd::string groupName, PositioningMode::Type type)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPositioning, name, type);
    }

    void AudioEngineMARequestHandler::SetSoundGroupPanMode(AZStd::string groupName, PanMode::Type type)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPanMode, name, type);
    }

    void AudioEngineMARequestHandler::SetSoundGroupDopplerFactor(AZStd::string groupName, float factor)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupDopplerFactor, name, factor);
    }

    void AudioEngineMARequestHandler::SetSoundGroupAttenuationCone(AZStd::string groupName, float innerAngle,
        float outerAngle, float outerGrain)
    {
        AZ::Name name(groupName.c_str());
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupAttenuationCone, name, innerAngle, outerAngle, outerGrain);
    }
} // namespace AudioEngineMA