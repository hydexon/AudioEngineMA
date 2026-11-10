#pragma once

#include <AudioEngineMA/AudioEngineMABus.h>

namespace AudioEngineMA
{
    class AudioEngineMARequestHandler : public AudioEngineMARequestBus::Handler
    {
    public:
        AudioEngineMARequestHandler();
        ~AudioEngineMARequestHandler();

        static void Reflect(AZ::ReflectContext* context);

        void LoadSoundGroupLayout(AZStd::string defFilePath) override;
        void SetSoundGroupSpatializationEnabled(AZStd::string groupName, bool value) override;
        void SetSoundGroupVolume(AZStd::string groupName, float volume) override;
        void SetSoundGroupPitch(AZStd::string groupName, float pitch) override;
        void SetSoundGroupPan(AZStd::string groupName, float pan) override;
        void SetSoundGroupRolloff(AZStd::string groupName, float rolloff) override;
        void SetSoundGroupMinDistance(AZStd::string groupName, float minDistance) override;
        void SetSoundGroupMaxDistance(AZStd::string groupName, float maxDistance) override;
        void SetSoundGroupMinGains(AZStd::string groupName, float minGains) override;
        void SetSoundGroupMaxGains(AZStd::string groupName, float maxGains) override;
        void SetSoundGroupAttenuationMode(AZStd::string groupName, AttenuationMode::Type type) override;
        void SetSoundGroupPositioning(AZStd::string groupName, PositioningMode::Type type) override;
        void SetSoundGroupPanMode(AZStd::string groupName, PanMode::Type type) override;
        void SetSoundGroupDopplerFactor(AZStd::string groupName,float factor) override;
        void SetSoundGroupAttenuationCone(AZStd::string groupName, float innerAngle, float outerAngle, float outerGrain) override;

    };
}