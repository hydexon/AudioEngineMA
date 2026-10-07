#pragma once

#include <AudioAllocators.h>
#include <AzCore/Name/Name.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/IO/Path/Path.h>

#include <miniaudio.h>

namespace AudioEngineMA {

class SoundGroupManager;

struct SoundGroupData;

class SoundGroup
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroup);
    AZ_DISABLE_COPY_MOVE(SoundGroup);

    SoundGroup(SoundGroup* parent, ma_engine* engine, const AZ::Name& name);
    SoundGroup(SoundGroup* parent, ma_engine* engine, const SoundGroupData& data);

    const bool IsValid() { return m_isValid; }
    const bool IsRootGroup() { return !m_parent; }
    SoundGroup* GetParentGroup() { return m_parent;    }
    ma_sound_group* GetMAGroup() { return &m_soundGrp; }
private:
    bool m_isValid;
    SoundGroup*    m_parent;
    ma_engine*     m_engine;
    ma_sound_group m_soundGrp;
    AZ::Name m_name;
};

class SoundGroupManager
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroupManager);
    AZ_DISABLE_COPY_MOVE(SoundGroupManager);

    SoundGroupManager(ma_engine* engine);
    ~SoundGroupManager();

    void Reset();
    bool IsSoundGroupExist(const AZ::Name& name);
    SoundGroup* GetSoundGroup(const AZ::Name& name);

    bool LoadGroupDefinitions(AZ::IO::PathView definitionFilePath);
    //bool SaveGroupDefinitions(AZ::IO::PathView definitionFilePath);
private:
    ma_engine* m_engine;
    void BuildSoundGroupTree(const SoundGroupData& data, SoundGroup* parent);
    AZStd::unordered_map<AZ::Name, AZStd::unique_ptr<SoundGroup>> m_groups;
};

}