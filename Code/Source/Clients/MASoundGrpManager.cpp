#include "MASoundGrpManager.h"
#include "SoundGroupData.h"
#include "Common_MA.h"

namespace AudioEngineMA
{

SoundGroup::SoundGroup(SoundGroup *parent, ma_engine *engine, const AZ::Name &name)
    : m_parent(parent)
    , m_name(name)
    , m_engine(engine)
{
    if(ma_sound_group_init(m_engine, 0, parent ? parent->GetMAGroup() : nullptr, &m_soundGrp) != MA_SUCCESS)
    {
        AZ_Error(Constants::LogWindow, false, "Error trying to initialize an sound group (with no data definitions)");
        m_isValid = false;
        return;
    }

    m_isValid = true;
}

SoundGroup::SoundGroup(SoundGroup *parent, ma_engine *engine, const SoundGroupData &data)
    : m_parent(parent)
    , m_name(data.m_groupName)
    , m_engine(engine)
{
    ma_uint32 flags = 0;
    flags ^= (data.m_noDefaultAttachment? MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT : 0U);
    flags ^= (data.m_noSpatialization ? MA_SOUND_FLAG_NO_SPATIALIZATION : 0U);
    flags ^= (data.m_noPitch ? MA_SOUND_FLAG_NO_PITCH : 0U);

    if(ma_sound_group_init(m_engine, flags, parent ? parent->GetMAGroup() : nullptr, &m_soundGrp) == MA_SUCCESS)
    {
        ma_sound_group_set_volume(&m_soundGrp, data.m_volume);
        ma_sound_group_set_pitch(&m_soundGrp, data.m_pitch);
        ma_sound_group_set_pan(&m_soundGrp, data.m_pan);
        m_isValid = true;
    }
    else {
        m_isValid = false;
    }
}

bool SoundGroupManager::IsSoundGroupExist(const AZ::Name &name)
{
    return m_groups.find(name) != m_groups.end();
}

SoundGroup *SoundGroupManager::GetSoundGroup(const AZ::Name &name)
{
    const auto entry = m_groups.find(name);
    if(entry == m_groups.end())
    {
        return nullptr;
    }

    return entry->second.get();
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
        BuildSoundGroupTree(rootGroup, nullptr);
    }

    return true;
}

void SoundGroupManager::BuildSoundGroupTree(const SoundGroupData &data, SoundGroup *parent)
{
    auto newGroup = AZStd::make_unique<SoundGroup>(parent, m_engine, data);

    if(newGroup->IsValid())
    {
        m_groups[data.m_groupName] = AZStd::move(newGroup);
        for(const SoundGroupData& childData : data.m_children)
        {
            BuildSoundGroupTree(childData, m_groups[data.m_groupName].get());
        }
    }
}


}