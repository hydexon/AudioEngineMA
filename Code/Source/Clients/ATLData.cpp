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

ma_attenuation_model AttenuationMode::ToMAModel(Type type)
{
    switch(type) {
    case AttenuationMode::None: return ma_attenuation_model_none;
    case AttenuationMode::Linear: return ma_attenuation_model_linear;
    case AttenuationMode::Inverse: return ma_attenuation_model_inverse;
    case AttenuationMode::Exponential: return ma_attenuation_model_exponential;
    default: return ma_attenuation_model_none;
    }
}

const char* PanMode::ToString(Type type)
{
    static const char* strings[PanMode::Count] = { "Balanced", "Pan" };
    return strings[type];
}

PanMode::Type PanMode::FromString(const char* str)
{
    if(AZ::StringFunc::Equal(str, "Balanced"))
    {
        return PanMode::Balance;
    }
    else if(AZ::StringFunc::Equal(str, "Pan"))
    {
        return PanMode::Pan;
    }

    return PanMode::Balance;
}

ma_pan_mode PanMode::ToMA(Type type)
{
    if(type == PanMode::Pan)
        return ma_pan_mode_pan;

    return ma_pan_mode_balance;
}

void AudioFileTriggerParameters::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{

}

void AudioFileTriggerParameters::WriteToXML(AZ::rapidxml::xml_node<char> &node, AZ::rapidxml::memory_pool<> &xmlAlloc) const
{

}



} // namespace AudioEngineMA