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

const char *PositioningMode::ToString(Type type)
{
    static const char* strings[PositioningMode::Count] = { "Absolute", "Relative" };
    return strings[type];
}

PositioningMode::Type PositioningMode::FromString(const char *str)
{
    if(AZ::StringFunc::Equal(str, "Absolute"))
    {
        return PositioningMode::Absolute;
    }
    else if(AZ::StringFunc::Equal(str, "Pan"))
    {
        return PositioningMode::Relative;
    }

    return PositioningMode::Absolute;

}

ma_positioning PositioningMode::ToMA(Type type)
{
    if(type == PositioningMode::Relative)
        return ma_positioning_relative;

    return ma_positioning_absolute;
}


void AudioFileTriggerParameters::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{
    auto triggerParamsNode = node.first_node(TagTriggerParams);
    if(!triggerParamsNode)
        return;

    auto cnode = triggerParamsNode->first_node(SoundAction::Tag);
    if(cnode)
    {
        m_soundAction = SoundAction::FromString(cnode->value());
    }

    cnode = triggerParamsNode->first_node(TagGroupName);
    if(cnode)
    {
        m_soundGroupName = cnode->value();
    }

    cnode = triggerParamsNode->first_node(AttenuationMode::Tag);
    if(cnode)
    {
        m_attenuationMode = AttenuationMode::FromString(cnode->value());
    }

    cnode = triggerParamsNode->first_node(PanMode::Tag);
    if(cnode)
    {
        m_panMode = PanMode::FromString(cnode->value());
    }

    cnode = triggerParamsNode->first_node(PositioningMode::Tag);
    if(cnode)
    {
        m_positioning = PositioningMode::FromString(cnode->value());
    }


    cnode = triggerParamsNode->first_node(TagRolloff);
    if(cnode)
    {
        m_rolloff = AZ::StringFunc::ToFloat(cnode->value());
    }

    cnode = triggerParamsNode->first_node(TagPitch);
    if(cnode)
    {
        m_pitch = AZ::StringFunc::ToFloat(cnode->value());
    }


    cnode = triggerParamsNode->first_node(TagPan);
    if(cnode)
    {
        m_pan = AZ::StringFunc::ToFloat(cnode->value());
    }

    cnode = triggerParamsNode->first_node(TagDopplerFactor);
    if(cnode)
    {
        m_dopplerFactor = AZ::StringFunc::ToFloat(cnode->value());
    }

    cnode = triggerParamsNode->first_node(TagVolumeLinear);
    if(cnode)
    {
        m_volume = AZ::StringFunc::ToFloat(cnode->value());
    }

    cnode = triggerParamsNode->first_node(TagLooping);
    if(cnode)
    {
        m_looping = AZ::StringFunc::ToBool(cnode->value());
    }

    cnode = triggerParamsNode->first_node(TagSpatialization);
    if(cnode)
    {
        m_spatialization = AZ::StringFunc::ToBool(cnode->value());
    }

    cnode = triggerParamsNode->first_node(TagDistances);
    if(cnode)
    {
        auto attr = cnode->first_attribute("Min");
        if(attr)
        {
            m_minDistance = AZ::StringFunc::ToFloat(attr->value());
        }

        attr = cnode->first_attribute("Max");
        if(attr)
        {
            m_maxDistance = AZ::StringFunc::ToFloat(attr->value());
        }
    }

    cnode = triggerParamsNode->first_node(TagGains);
    if(cnode)
    {
        auto attr = cnode->first_attribute("Min");
        if(attr)
        {
            m_minGains = AZ::StringFunc::ToFloat(attr->value());
        }

        attr = cnode->first_attribute("Max");
        if(attr)
        {
            m_maxGains = AZ::StringFunc::ToFloat(attr->value());
        }
    }


    cnode = triggerParamsNode->first_node(TagCone);
    if(cnode)
    {
        auto attr = cnode->first_attribute("innerAngle");
        if(attr)
        {
            m_coneinnerAngle = AZ::StringFunc::ToFloat(attr->value());
        }

        attr = cnode->first_attribute("outerAngle");
        if(attr)
        {
            m_coneOuterAngle = AZ::StringFunc::ToFloat(attr->value());
        }

        attr = cnode->first_attribute("outerGrain");
        if(attr)
        {
            m_coneOuterGrain = AZ::StringFunc::ToFloat(attr->value());
        }

    }
}

void AudioFileTriggerParameters::WriteToXML(AZ::rapidxml::xml_node<char> &node, AZ::rapidxml::memory_pool<> &xmlAlloc) const
{
    //Append subnode to parent "node".
    auto subnode = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagTriggerParams);

    {
        auto actionNode    = xmlAlloc.allocate_node(AZ::rapidxml::node_element, SoundAction::Tag, SoundAction::ToString(m_soundAction));
        auto soundGrpNode  = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagGroupName, m_soundGroupName.GetCStr());
        auto attenModeNode = xmlAlloc.allocate_node(AZ::rapidxml::node_element, AttenuationMode::Tag, AttenuationMode::ToString(m_attenuationMode));
        auto panModeNode   = xmlAlloc.allocate_node(AZ::rapidxml::node_element, PanMode::Tag, PanMode::ToString(m_panMode));
        auto posNode       = xmlAlloc.allocate_node(AZ::rapidxml::node_element, PositioningMode::Tag, PositioningMode::ToString(m_positioning));
        auto rolloffNode   = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagRolloff, xmlAlloc.allocate_string(AZStd::to_string(m_rolloff).c_str()));
        auto dopplerNode   = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagDopplerFactor, xmlAlloc.allocate_string(AZStd::to_string(m_dopplerFactor).c_str()));
        auto loopingNode   = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagLooping, xmlAlloc.allocate_string(AZStd::to_string(m_looping).c_str()));
        auto spatialNode   = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagSpatialization, xmlAlloc.allocate_string(AZStd::to_string(m_spatialization).c_str()));
        auto volumeNode    = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagVolumeLinear, xmlAlloc.allocate_string(AZStd::to_string(m_volume).c_str()));
        auto panNode       = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagPan, xmlAlloc.allocate_string(AZStd::to_string(m_pan).c_str()));
        auto pitchNode     = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagPitch, xmlAlloc.allocate_string(AZStd::to_string(m_pitch).c_str()));

        subnode->append_node(actionNode);
        subnode->append_node(soundGrpNode);
        subnode->append_node(attenModeNode);
        subnode->append_node(panModeNode);
        subnode->append_node(posNode);
        subnode->append_node(rolloffNode);
        subnode->append_node(dopplerNode);
        subnode->append_node(loopingNode);
        subnode->append_node(spatialNode);
        subnode->append_node(volumeNode);
        subnode->append_node(panNode);
        subnode->append_node(pitchNode);
    }

    {
        auto distancesNode = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagDistances);

        auto attr = xmlAlloc.allocate_attribute("Min", xmlAlloc.allocate_string(AZStd::to_string(m_minDistance).c_str()));
        distancesNode->append_attribute(attr);

        attr = xmlAlloc.allocate_attribute("Max", xmlAlloc.allocate_string(AZStd::to_string(m_maxDistance).c_str()));
        distancesNode->append_attribute(attr);

        subnode->append_node(distancesNode);
    }

    {
        auto attConeNode = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagCone);
        auto attr = xmlAlloc.allocate_attribute("innerAngle", xmlAlloc.allocate_string(AZStd::to_string(m_coneinnerAngle).c_str()));
        attConeNode->append_attribute(attr);

        attr = xmlAlloc.allocate_attribute("outerAngle", xmlAlloc.allocate_string(AZStd::to_string(m_coneOuterAngle).c_str()));
        attConeNode->append_attribute(attr);

        attr = xmlAlloc.allocate_attribute("outerGrain", xmlAlloc.allocate_string(AZStd::to_string(m_coneOuterGrain).c_str()));
        attConeNode->append_attribute(attr);

        subnode->append_node(attConeNode);
    }

    {
        auto gainsNode = xmlAlloc.allocate_node(AZ::rapidxml::node_element, TagGains);
        auto attr = xmlAlloc.allocate_attribute("Min", xmlAlloc.allocate_string(AZStd::to_string(m_minGains).c_str()));
        gainsNode->append_attribute(attr);

        attr = xmlAlloc.allocate_attribute("Max", xmlAlloc.allocate_string(AZStd::to_string(m_maxGains).c_str()));
        gainsNode->append_attribute(attr);

        subnode->append_node(gainsNode);
    }

    node.append_node(subnode);


}

const char *AudioFileRTPC::ToString(Type type)
{
    static const char* strings[AudioFileRTPC::Count] = {"Volume", "Pitch", "Seek"};
    return strings[type];
}

AudioFileRTPC::Type AudioFileRTPC::FromString(const char *str)
{
    if(AZ::StringFunc::Equal(str, "Volume"))
    {
        return AudioFileRTPC::Volume;
    }
    else if(AZ::StringFunc::Equal(str, "Pitch"))
    {
        return AudioFileRTPC::Pitch;
    }
    else if(AZ::StringFunc::Equal(str, "Seek"))
    {
        return AudioFileRTPC::Seek;
    }

    return AudioFileRTPC::Count;
}

const char *GlobalRTPC::ToString(Type type)
{
    static const char* strings[GlobalRTPC::Count] = { "GlobalVolume" };
    return strings[type];
}

GlobalRTPC::Type GlobalRTPC::FromString(const char *str)
{
    if(AZ::StringFunc::Equal(str, "GlobalVolume"))
        return GlobalRTPC::GlobalVolume;

    return GlobalRTPC::Count;
}

bool AudioFileRTPCParams::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{
    auto attr = node.first_attribute(AudioFileRTPC::Tag);
    if(attr)
    {
        if(m_type == AudioFileRTPC::Count)
        {
            return false;
        }
        m_type = AudioFileRTPC::FromString(attr->value());
    }

    attr = node.first_attribute(TagPerObject);
    if(attr)
    {
        m_perObject = AZ::StringFunc::ToBool(attr->value());
    }

    attr = node.first_attribute(TagMult);
    if(attr)
    {
        m_mult = AZ::StringFunc::ToFloat(attr->value());
    }

    attr = node.first_attribute(TagShift);
    if(attr)
    {
        m_shift = AZ::StringFunc::ToFloat(attr->value());
    }
    return true;
}

void AudioFileRTPCParams::WriteToXML(AZ::rapidxml::xml_node<char> &node, AZ::rapidxml::memory_pool<> &xmlAlloc) const
{
    auto attr = xmlAlloc.allocate_attribute(AudioFileRTPC::Tag, AudioFileRTPC::ToString(m_type));
    node.append_attribute(attr);

    attr = xmlAlloc.allocate_attribute(TagPerObject, xmlAlloc.allocate_string(AZStd::to_string(m_perObject).c_str()));
    node.append_attribute(attr);

    attr = xmlAlloc.allocate_attribute(TagMult, xmlAlloc.allocate_string(AZStd::to_string(m_mult).c_str()));
    node.append_attribute(attr);

    attr = xmlAlloc.allocate_attribute(TagShift, xmlAlloc.allocate_string(AZStd::to_string(m_shift).c_str()));
    node.append_attribute(attr);
}

bool GlobalRTPCParams::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{
    auto attr = node.first_attribute(AudioFileRTPC::Tag);
    if(attr)
    {
        m_type = GlobalRTPC::FromString(attr->value());
        if(m_type == GlobalRTPC::Count)
        {
            return false;
        }
    }

    attr = node.first_attribute(TagMult);
    if(attr)
    {
        m_mult = AZ::StringFunc::ToFloat(attr->value());
    }

    attr = node.first_attribute(TagShift);
    if(attr)
    {
        m_shift = AZ::StringFunc::ToFloat(attr->value());
    }

    return true;
}

void GlobalRTPCParams::WriteToXML(AZ::rapidxml::xml_node<char> &node, AZ::rapidxml::memory_pool<> &xmlAlloc) const
{
    auto attr = xmlAlloc.allocate_attribute(GlobalRTPC::Tag, GlobalRTPC::ToString(m_type));
    node.append_attribute(attr);
    attr = xmlAlloc.allocate_attribute(TagMult, xmlAlloc.allocate_string(AZStd::to_string(m_mult).c_str()));
    node.append_attribute(attr);

    attr = xmlAlloc.allocate_attribute(TagShift, xmlAlloc.allocate_string(AZStd::to_string(m_shift).c_str()));
    node.append_attribute(attr);
}


} // namespace AudioEngineMA