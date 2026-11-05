#pragma once

#include <AzCore/IO/Path/Path.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/XML/rapidxml.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>

#include "ATLData.h"

namespace AudioEngineMA
{

struct SoundGroupParams
{
};

struct SoundGroupData
{
    AZ_TYPE_INFO(SoundGroupData, "{6C11E3AF-DDF5-48B7-BB8E-618695EDC1CC}");
    SoundGroupData() = default;
    ~SoundGroupData() = default;
    static void Reflect(AZ::ReflectContext* context);

    constexpr static const char* TagSoundGroupData = "SoundGroupData";

    void ReadFromXML(const AZ::rapidxml::xml_node<char>& node);
    void WriteToXML(AZ::rapidxml::xml_node<char>& node, AZ::rapidxml::memory_pool<>& xmlAlloc) const;

    AZ::Name m_groupName;

    bool m_noPitch = false;
    bool m_noSpatialization = false;

    float m_volume = 1.0f;
    float m_pitch  = 1.0f;
    float m_pan    = 1.0f;
    float m_rolloff = 1.0f;
    float m_coneinnerAngle = (2 * 3.14159265358979323846264f);
    float m_coneOuterAngle = (2 * 3.14159265358979323846264f);
    float m_coneOuterGrain = 1.0f;
    float m_minDistance = 1.0f;
    float m_maxDistance = FLT_MAX;
    float m_dopplerFactor = 1.0f;
    float m_minGains = 0.1f;
    float m_maxGains = 1.0f;
    bool m_spatializationEnable = true;

    AttenuationMode::Type m_attenuationMode = AttenuationMode::Inverse;
    PositioningMode::Type m_positioningType = PositioningMode::Absolute;
    PanMode::Type m_panMode = PanMode::Balance;

    AZStd::vector<SoundGroupData> m_children;
};

struct SoundGroupLayoutData
{
    AZ_TYPE_INFO(SoundGroupLayoutData, "{5C69ED62-14CE-43DB-92D1-4ED02B307942}");
    SoundGroupLayoutData()  = default;
    ~SoundGroupLayoutData() = default;
    static void Reflect(AZ::ReflectContext* context);

    constexpr static const char* TagSoundGroup = "SoundGroup";

    bool Save(AZ::IO::PathView filePath);
    bool Load(AZ::IO::PathView filePath);

    void ReadFromXML(const AZ::rapidxml::xml_node<char>& node);
    void WriteToXML(AZ::rapidxml::xml_node<char>& node, AZ::rapidxml::memory_pool<>& xmlAlloc) const;

    AZStd::vector<SoundGroupData> m_groups;
};

}