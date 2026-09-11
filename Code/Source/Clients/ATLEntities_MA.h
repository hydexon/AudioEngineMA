#pragma once

#include <ATLEntityData.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/Name/Name.h>

#include <miniaudio.h>

namespace AudioEngineMA
{

struct SATLAudioFileEntryData_MA : Audio::IATLAudioFileEntryData
{
    AZ::IO::FixedMaxPath m_audioFilePath;
};

struct ActiveMASoundData {
    ActiveMASoundData() = default;
    ~ActiveMASoundData() = default;

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
    //AudioFile Trigger Params:
    AZ::Name m_soundGroupName;


};

struct SATLEventData_MA : public Audio::IATLEventData
{
    ma_sound* m_soundInstance;
};


}