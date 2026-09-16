#include "MASoundGrpManager.h"

namespace AudioEngineMA
{

SoundGroupManager::SoundGroupManager(ma_engine *engine)
{

}

SoundGroupManager::~SoundGroupManager()
{

}

void SoundGroupManager::Reset()
{

}

SoundGroup *SoundGroupManager::CreateSoundGroup(const AZ::Name &grpName)
{

    auto group = AZStd::make_unique<SoundGroup>(*this, m_engine, nullptr, grpName);
    if(!group)
    {
        return nullptr;
    }

    m_soundGroups.emplace_back(AZStd::move(group));
    return m_soundGroups.back().get();
}

bool SoundGroupManager::DestroyGroup(const AZ::Name &grpName)
{
    return true;
}

bool SoundGroupManager::DestroyGroup(int32_t grpIndex)
{
    return true;
}

bool SoundGroupManager::LoadGroupDefinitions(AZ::IO::PathView definitionFilePath)
{
    return true;
}

bool SoundGroupManager::SaveGroupDefinitions(AZ::IO::PathView definitionFilePath)
{
    return true;
}

SoundGroup::SoundGroup(SoundGroupManager &manager, ma_engine *engine, ma_sound_group* parent, const AZ::Name &name)
    : m_grpManager(manager)
    , m_name(name)
    , m_group(nullptr)
{
    ma_sound_group_init(engine, 0, parent, m_group);
}



}