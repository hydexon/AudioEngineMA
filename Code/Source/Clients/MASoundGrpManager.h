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
    friend class SoundGroupManager;

    SoundGroup(SoundGroup* parent, ma_engine* engine, const AZ::Name& name, ma_uint32 flags = 0);

    bool Initialize();
    void Uninitialize();

    const AZ::Name& GetName() const { return m_name; }
    ma_sound_group* GetMASoundGroup() { return &m_group; }
    SoundGroup* GetParent() { return m_parent; }
    AZStd::vector<AZStd::unique_ptr<SoundGroup>>& GetChildren();

private:
    ma_engine* m_engine;
    ma_sound_group m_group;

    bool m_isInitialized;
    AZ::Name m_name;
    ma_uint32 m_flags;
    SoundGroup* m_parent;
    AZStd::vector<AZStd::unique_ptr<SoundGroup>> m_children;
};

class SoundGroupManager
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroupManager);
    AZ_DISABLE_COPY_MOVE(SoundGroupManager)

    SoundGroupManager(ma_engine* engine);
    ~SoundGroupManager();

    void Reset();

    SoundGroup* GetSoundGroup(const AZ::Name& name);
    SoundGroup* GetRootSoundGroupAtIndex(const int index);
    const bool SoundGroupExists(const AZ::Name& name);
    SoundGroup* CreateSoundGroup(const AZ::Name& name, SoundGroup* parent = nullptr, bool noDefaultAttach = false, bool noSpatial = false, bool noPitch = false);
    bool RenameSoundGroup(const AZ::Name& oldName, const AZ::Name& newName);

    bool LoadGroupDefinitions(AZ::IO::PathView definitionFilePath);
    //bool SaveGroupDefinitions(AZ::IO::PathView definitionFilePath);

    size_t GetRootNodeCount() const;
private:
    ma_engine* m_engine;
    AZStd::vector<AZStd::unique_ptr<SoundGroup>> m_groups;
    AZStd::unordered_map<AZ::Name, SoundGroup*> m_groupsMap;

    ma_uint32 GetFlagsFromBools(bool noDefaultAttach, bool noSpatial, bool noPitch) const;
    AZStd::unique_ptr<SoundGroup> PopulateSoundGroup(const SoundGroupData& data, SoundGroup* parent);
};

}