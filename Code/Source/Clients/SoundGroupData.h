#pragma once

#include <AzCore/IO/Path/Path.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>

namespace AudioEngineMA
{


struct SoundGroupData
{
    AZ_TYPE_INFO(SoundGroupData, "{6C11E3AF-DDF5-48B7-BB8E-618695EDC1CC}");
    SoundGroupData()  = default;
    ~SoundGroupData() = default;
    static void Reflect(AZ::ReflectContext* context);

    AZ::Name m_groupName;

    bool m_noPitch = false;
    bool m_noSpatialization = false;
    bool m_noDefaultAttachment = false;

    float m_volume = 0.0f;
    float m_pitch  = 1.0f;
    float m_pan    = 0.0f;

    AZStd::vector<SoundGroupData> m_children;
};

struct SoundGroupLayoutData
{
    AZ_TYPE_INFO(SoundGroupLayoutData, "{5C69ED62-14CE-43DB-92D1-4ED02B307942}");
    SoundGroupLayoutData()  = default;
    ~SoundGroupLayoutData() = default;
    static void Reflect(AZ::ReflectContext* context);

    bool Save(AZ::IO::PathView filePath);
    bool Load(AZ::IO::PathView filePath);

    AZStd::vector<SoundGroupData> m_groups;
};

}