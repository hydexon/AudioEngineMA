#include "MASoundGrpManager.h"
#include "SoundGroupData.h"
#include "Common_MA.h"

namespace AudioEngineMA
{

SoundGroup::SoundGroup(SoundGroupManager& manager, SoundGroup* parent, ma_engine* engine, const AZ::Name& name)
    : m_name(name)
    , m_parent(parent)
{
    ma_sound_group* parentGrp = parent ? parent->GetMAGroup() : nullptr;
    ma_result r = ma_sound_group_init(engine, 0, parentGrp, &m_soundGrp);
    if(r != MA_SUCCESS)
    {
        //Return failure.
        AZ_Error(Constants::LogWindow, false, "Unable to init sound group with name: '%s'", name.GetCStr());
    }

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
        SoundGroup* grp = nullptr;
        bool isMasterGroup = rootGroup.m_groupName == AZ::Name(Constants::MasterGroupName);
        if(isMasterGroup)
        {
        }
        else
        {
            grp = CreateSoundGroups(rootGroup.m_groupName, nullptr, rootGroup.m_children);
            AZ_UNUSED(grp);
        }

    }
    return true;
}

bool SoundGroupManager::SaveGroupDefinitions(AZ::IO::PathView definitionFilePath)
{
    return true;
}

SoundGroup *SoundGroupManager::CreateSoundGroups(const AZ::Name &name, SoundGroup *parent, const AZStd::vector<SoundGroupData>& childData)
{
    auto group = AZStd::make_unique<SoundGroup>(*this, parent, m_engine, name);
    for(auto child : childData)
    {
        CreateSoundGroups(child.m_groupName, group.get(), child.m_children);
    }
    return nullptr;
}




}