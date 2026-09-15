#include "ATLData.h"

#include <AzCore/StringFunc/StringFunc.h>
#include <AzCore/std/string/conversions.h>

namespace AudioEngineMA
{

const char *SoundAction::ToString(Type type)
{
    static const char* strings[SoundAction::Count] = { "Start", "Stop", "Pause", "Resume" };

    return strings[type];
}

SoundAction::Type SoundAction::FromString(const char *str)
{
    if(AZ::StringFunc::Equal(str, "Start"))
    {
        return SoundAction::Start;
    }
    else if(AZ::StringFunc::Equal(str, "Stop"))
    {
        return SoundAction::Stop;
    }
    else if(AZ::StringFunc::Equal(str, "Pause"))
    {
        return SoundAction::Pause;
    }
    else if(AZ::StringFunc::Equal(str, "Resume"))
    {
        return SoundAction::Resume;
    }
    else {
        return SoundAction::Count;
    }
}

const char *AttenuationMode::ToString(Type type)
{
    static const char* strings[AttenuationMode::Count] = { "None", "Inverse", "Linear", "Exponential" };
    return strings[type];
}

AttenuationMode::Type AttenuationMode::FromString(const char *str)
{
    if(AZ::StringFunc::Equal(str, "None"))
    {
        return AttenuationMode::None;
    }
    else if(AZ::StringFunc::Equal(str, "Inverse"))
    {
        return AttenuationMode::Inverse;
    }
    else if(AZ::StringFunc::Equal(str, "Linear"))
    {
        return AttenuationMode::Linear;
    }
    else if(AZ::StringFunc::Equal(str, "Exponential"))
    {
        return AttenuationMode::Exponential;
    }
    else {
        return AttenuationMode::Count;
    }
}

void AudioFileTriggerParameters::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{

}

void AudioFileTriggerParameters::WriteToXML(AZ::rapidxml::xml_node<char> &node, AZ::rapidxml::memory_pool<> &xmlAlloc) const
{

}


} // namespace AudioEngineMA