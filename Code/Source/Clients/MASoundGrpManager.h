#pragma once

#include <AudioAllocators.h>
#include <AzCore/Name/Name.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>
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

    SoundGroup(SoundGroupManager& manager, SoundGroup* parent, ma_engine* engine, const AZ::Name& name);

    void AddChildren(SoundGroup* group);
    const bool IsRootGroup() { return !m_parent; }
    SoundGroup* GetParentGroup() { return m_parent;    }
    ma_sound_group* GetMAGroup() { return &m_soundGrp; }
    AZStd::vector<SoundGroup>& GetChildren() { return m_children; }

private:
    SoundGroup*    m_parent;
    ma_sound_group m_soundGrp;
    AZ::Name m_name;
    AZStd::vector<SoundGroup> m_children;
};

class SoundGroupManager
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroupManager);
    AZ_DISABLE_COPY_MOVE(SoundGroupManager);

    SoundGroupManager(ma_engine* engine);
    ~SoundGroupManager();

    void Reset();

    bool LoadGroupDefinitions(AZ::IO::PathView definitionFilePath);
    bool SaveGroupDefinitions(AZ::IO::PathView definitionFilePath);


private:
    ma_engine* m_engine;
    AZStd::vector<AZStd::unique_ptr<SoundGroup>> m_rootSoundGroups;
};

}