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
            behaviorContext->EBus<AudioEngineMARequestBus>("MiniAudioSoundGroupRequestBus")
                ->Event("LoadSoundGroupLayout", &AudioEngineMARequestBus::Events::LoadSoundGroupLayout, {{{"Sound Group Layout File", ""}}})
                ->Event("SetSoundGroupSpatializationEnabled", &AudioEngineMARequestBus::Events::SetSoundGroupSpatializationEnabled)
                ->Event("SetSoundGroupVolume", &AudioEngineMARequestBus::Events::SetSoundGroupVolume, {{{"Group Name", ""}, {"Volume",""}}})
                ->Event("SetSoundGroupPitch", &AudioEngineMARequestBus::Events::SetSoundGroupPitch, {{{"Group Name", ""}, {"Pitch",""}}})
                ->Event("SetSoundGroupPan", &AudioEngineMARequestBus::Events::SetSoundGroupPan, {{{"Group Name", ""}, {"Pan",""}}})
                ->Event("SetSoundGroupRolloff", &AudioEngineMARequestBus::Events::SetSoundGroupRolloff,{{{"Group Name", ""}, {"Rolloff",""}}})
                ->Event("SetSoundGroupMinDistance", &AudioEngineMARequestBus::Events::SetSoundGroupMinDistance, {{{"Group Name", ""}, {"Min Distance",""}}})
                ->Event("SetSoundGroupMaxDistance", &AudioEngineMARequestBus::Events::SetSoundGroupMaxDistance, {{{"Group Name", ""}, {"Max Distance",""}}})
                ->Event("SetSoundGroupMinGains", &AudioEngineMARequestBus::Events::SetSoundGroupMinGains, {{{"Group Name", ""}, {"Min Gains",""}}})
                ->Event("SetSoundGroupMaxGains", &AudioEngineMARequestBus::Events::SetSoundGroupMaxGains, {{{"Group Name", ""}, {"Max Gains",""}}})
                ->Event("SetSoundGroupAttenuationMode", &AudioEngineMARequestBus::Events::SetSoundGroupAttenuationMode, {{{"Group Name", ""}, {"Mode",""}}})
                ->Event("SetSoundGroupPositioning", &AudioEngineMARequestBus::Events::SetSoundGroupPositioning, {{{"Group Name", ""}, {"Mode",""}}})
                ->Event("SetSoundGroupPanMode", &AudioEngineMARequestBus::Events::SetSoundGroupPanMode, {{{"Group Name", ""}, {"Mode",""}}})
                ->Event("SetSoundGroupDopplerFactor", &AudioEngineMARequestBus::Events::SetSoundGroupDopplerFactor, {{{"Group Name", ""}, {"Doppler Factor",""}}})
                ->Event("SetSoundGroupAttenuationCone", &AudioEngineMARequestBus::Events::SetSoundGroupAttenuationCone, {{{"Group Name", ""}, {"Inner Angle",""}, {"Outer Angle",""}, {"Outer Grain",""}}});
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