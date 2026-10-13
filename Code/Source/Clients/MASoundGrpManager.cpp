#include "MASoundGrpManager.h"
#include "SoundGroupData.h"
#include "Common_MA.h"

namespace AudioEngineMA
{


SoundGroup::SoundGroup(SoundGroup *parent, ma_engine *engine, const AZ::Name &name, ma_uint32 flags)
    : m_parent(parent)
    , m_name(name)
    , m_flags(flags)
    , m_engine(engine)
    , m_isInitialized(false)
{
}

bool SoundGroup::Initialize()
{
    ma_result res = ma_sound_group_init(m_engine, m_flags, m_parent ? m_parent->GetMASoundGroup() : nullptr, &m_group);
    if(res != MA_SUCCESS)
    {
        AZ_Error(Constants::LogWindow, res == MA_SUCCESS, "Error trying creating ma_sound_group!: %d", res);
        return false;
    }

    m_isInitialized = true;
    return true;
}

void SoundGroup::Uninitialize()
{
    ma_sound_group_stop(&m_group);
    ma_sound_group_uninit(&m_group);

    m_isInitialized = false;
}

AZStd::vector<AZStd::unique_ptr<SoundGroup> > &SoundGroup::GetChildren()
{
    return m_children;
}

SoundGroupManager::SoundGroupManager(ma_engine *engine)
    : m_engine(engine)
{
    
}

SoundGroupManager::~SoundGroupManager()
{

}

void SoundGroupManager::Reset()
{
    m_groups.clear();
    m_groupsMap.clear();
}

SoundGroup *SoundGroupManager::GetSoundGroup(const AZ::Name &name)
{
    auto grp = m_groupsMap.find(name);
    return grp != m_groupsMap.end() ? grp->second : nullptr;
}

SoundGroup *SoundGroupManager::GetRootSoundGroupAtIndex(const int index)
{
    return m_groups[index].get();
}

const bool SoundGroupManager::SoundGroupExists(const AZ::Name &name)
{
    return GetSoundGroup(name) != nullptr;
}

SoundGroup *SoundGroupManager::CreateSoundGroup(const AZ::Name &name, SoundGroup *parent, bool noDefaultAttach, bool noSpatial, bool noPitch)
{
    ma_uint32 flags = GetFlagsFromBools(noDefaultAttach, noSpatial, noPitch);
    auto newGrp = AZStd::make_unique<SoundGroup>(parent, m_engine, name, flags);

    if(!newGrp->Initialize())
    {
        return nullptr;
    }

    SoundGroup* grp = nullptr;
    if(!parent) //No parent, must be root.
    {
        m_groups.emplace_back(AZStd::move(newGrp));
        grp = m_groups.back().get();

    }
    else
    {
        parent->GetChildren().emplace_back(AZStd::move(newGrp));
        grp = parent->GetChildren().back().get();
    }

    m_groupsMap[name] = grp;
    return grp;
}

bool SoundGroupManager::RenameSoundGroup(const AZ::Name &oldName, const AZ::Name &newName)
{
    if( oldName == newName )
        return false;

    if(newName.IsEmpty())
    {
        AZ_Error(Constants::LogWindow, false, "Unable to rename sound group \"%s\" because the name supplied is empty.", oldName.GetCStr());
        return false;
    }

    if(SoundGroupExists(newName))
    {
        AZ_Error(
            Constants::LogWindow, false, "Unable to change the name of the sound group from \"%s\" to \"%s\". A sound group with this name already exists.",
            oldName.GetCStr(), newName.GetCStr());

        return false;
    }

    SoundGroup* group = GetSoundGroup(oldName);
    if (!group)
    {
        AZ_Error(Constants::LogWindow, false, "Unable to rename sound group \"%s\" because it doesn't exist.", oldName.GetCStr());
        return false;
    }

    //The map cache must reflect this, otherwise it will fail.
    auto nodeHandle = m_groupsMap.extract(oldName);
    if(nodeHandle.empty())
    {
        AZ_Error(Constants::LogWindow, false, "Unable to rename sound group \"%s\", it's lookup cache map entry is non-existent.", oldName.GetCStr());
        return false;
    }

    group->m_name = newName;
    nodeHandle.key() = newName;
    m_groupsMap.insert(AZStd::move(nodeHandle));

    return true;
}

bool SoundGroupManager::LoadGroupDefinitions(AZ::IO::PathView definitionFilePath)
{
    SoundGroupLayoutData layout;
    if(!layout.Load(definitionFilePath))
    {
        return false;
    }

    Reset();

    for(const auto& rootGroup : layout.m_groups)
    {
        auto root = PopulateSoundGroup(rootGroup, nullptr);
        if(!root)
            continue;

        m_groups.emplace_back(AZStd::move(root));
    }

    return true;
}

size_t SoundGroupManager::GetRootNodeCount() const
{
    return m_groups.size();
}

ma_uint32 SoundGroupManager::GetFlagsFromBools(bool noDefaultAttach, bool noSpatial, bool noPitch) const
{
    ma_uint32 flags = 0;
    flags ^= (noDefaultAttach ? MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT : 0U);
    flags ^= (noSpatial ? MA_SOUND_FLAG_NO_SPATIALIZATION : 0U);
    flags ^= (noPitch ? MA_SOUND_FLAG_NO_PITCH : 0U);
    return flags;
}

AZStd::unique_ptr<SoundGroup> SoundGroupManager::PopulateSoundGroup(const SoundGroupData &data, SoundGroup *parent)
{
    ma_uint32 flags = GetFlagsFromBools(data.m_noDefaultAttachment, data.m_noSpatialization, data.m_noPitch);
    auto groupNode = AZStd::make_unique<SoundGroup>(parent, m_engine, data.m_groupName, flags);
    if(!groupNode->Initialize())
    {
        return nullptr;
    }

    m_groupsMap[data.m_groupName] = groupNode.get();
    groupNode->GetChildren().reserve(data.m_children.size());

    //Init miniaudio-specific parameters:
    ma_sound_group_set_pan(groupNode->GetMASoundGroup(), data.m_pan);
    ma_sound_group_set_pitch(groupNode->GetMASoundGroup(), data.m_pitch);
    ma_sound_group_set_volume(groupNode->GetMASoundGroup(), data.m_volume);

    for(const auto& child : data.m_children)
    {
        groupNode->GetChildren().push_back(PopulateSoundGroup(child, groupNode.get()));
    }

    return groupNode;
}

}