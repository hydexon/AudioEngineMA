#pragma once

#include <IAudioConnection.h>
#include "../Clients/ATLData.h"

namespace AudioEngineMA
{
    class AudioFileToTriggerConnection : public AudioControls::IAudioConnection
    {
    public:
        explicit AudioFileToTriggerConnection(AudioControls::CID id)
            : AudioControls::IAudioConnection(id)
        {}

        ~AudioFileToTriggerConnection() = default;

        bool HasProperties() override { return true; }

        AudioFileTriggerParameters m_params;
    };

}