#pragma once

#include <ATLEntityData.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/Name/Name.h>

#include "ATLData.h"
#include <miniaudio.h>

namespace AudioEngineMA
{

struct SATLAudioFileEntryData_MA : Audio::IATLAudioFileEntryData
{
    AZ::IO::FixedMaxPath m_audioFilePath;
};

struct ActiveMASoundData {
    bool m_isPaused = false;
    ma_sound* m_sound;
    float m_volume = 0.0f;
};

struct SATLAudioObjectData_MA : public Audio::IATLAudioObjectData
{
    SATLAudioObjectData_MA() = default;
    ~SATLAudioObjectData_MA() = default;

    AZ::Vector3 m_position;
    AZStd::unordered_multimap<AZ::IO::FixedMaxPath, ActiveMASoundData> m_activeMASounds;
};

struct SATLTriggerImplData_MA : public Audio::IATLTriggerImplData
{
    AZ::IO::FixedMaxPath m_audioFilePath;
    AudioFileTriggerParameters m_audioTriggerParams;
};

struct SATLEventData_MA : public Audio::IATLEventData
{
    ma_sound* m_soundInstance = nullptr;
    bool m_isPlayingEvent = false;
};


}