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

    class AudioFileToRTPCConnection : public AudioControls::IAudioConnection
    {
    public:
        explicit AudioFileToRTPCConnection(AudioControls::CID id)
            : AudioControls::IAudioConnection(id)
        {}

        ~AudioFileToRTPCConnection() = default;

        bool HasProperties() override { return true; }

        AudioFileRTPCParams m_params;
    };

    class GlobalRTPCConnection : public AudioControls::IAudioConnection
    {
    public:
        explicit GlobalRTPCConnection(AudioControls::CID id)
            : AudioControls::IAudioConnection(id)
        {}

        ~GlobalRTPCConnection() = default;
        bool HasProperties() override { return true; }

        GlobalRTPCParams m_params;
    };

}