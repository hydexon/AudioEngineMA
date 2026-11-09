#include "SoundGroupManager.h"
#include "SoundGroupData.h"
#include "Common_MA.h"

namespace AudioEngineMA
{
SoundGroup::SoundGroup(ma_engine *engine, AZ::Name &name, SoundGroup *parent, SoundGroupData *srcdata)
    : m_engine(engine)
    , m_name(name)
    , m_parent(parent)
{
    ma_sound_group_init(m_engine, 0, parent ? &parent->m_group : nullptr, &m_group);

    //Apply properties:
    ApplyData(srcdata);
}

SoundGroup::~SoundGroup()
{
    for(auto sound : m_associatedSounds)
    {
        ma_sound_stop(sound);
    }
    ma_sound_group_uninit(&m_group);
}

bool SoundGroup::AddSound(ma_sound *sound)
{
    m_associatedSounds.push_back(sound);
    return true;
}

void SoundGroup::RemoveSound(ma_sound *sound)
{
    AZStd::erase(m_associatedSounds, sound);
}

void SoundGroup::ApplyData(SoundGroupData *srcdata)
{
    if(!srcdata)
        return;

    setSpatializationEnabled(srcdata->m_spatialization);
    setVolume(srcdata->m_volume);
    setPitch(srcdata->m_pitch);
    setPitch(srcdata->m_pan);
    setRolloff(srcdata->m_rolloff);
    setCone(srcdata->m_coneinnerAngle, srcdata->m_coneOuterAngle, srcdata->m_coneOuterGrain);
    setMinDistance(srcdata->m_minDistance);
    setMaxDistance(srcdata->m_maxDistance);
    setDopplerFactor(srcdata->m_dopplerFactor);
    setMinGains(srcdata->m_minGains);
    setMaxGains(srcdata->m_maxGains);
    setAttenuationMode(srcdata->m_attenuationMode);
    setPositioningMode(srcdata->m_positioningType);
    setPanMode(srcdata->m_panMode);
}

void SoundGroup::GatherData(SoundGroupData &dstData)
{
    dstData.m_spatialization = isSpatializationEnabled();
    dstData.m_groupName = name();
    dstData.m_volume = getVolume();
    dstData.m_pitch  = getPitch();
    dstData.m_rolloff = getRollOff();
    dstData.m_minDistance = getMinDistance();
    dstData.m_maxDistance = getMaxDistance();
    dstData.m_dopplerFactor = getDopplerFactor();
    dstData.m_minGains = getMinGains();
    dstData.m_maxGains = getMaxGains();
    dstData.m_attenuationMode = getAttenuationMode();
    dstData.m_positioningType = getPositioningMode();
    dstData.m_panMode = getPanMode();
    getCone(dstData.m_coneinnerAngle, dstData.m_coneOuterAngle, dstData.m_coneOuterGrain);
}

bool SoundGroup::isSpatializationEnabled()
{
    return ma_sound_group_is_spatialization_enabled(&m_group);
}

void SoundGroup::setSpatializationEnabled(bool enabled)
{
    ma_sound_group_set_spatialization_enabled(&m_group, enabled);
}

float SoundGroup::getVolume() const
{
    return ma_sound_group_get_volume(&m_group);
}

void SoundGroup::setVolume(float volume)
{
    ma_sound_group_set_volume(&m_group, volume);
}

float SoundGroup::getPitch() const
{
    return ma_sound_group_get_pitch(&m_group);
}

void SoundGroup::setPitch(float pitch)
{
    ma_sound_group_set_pitch(&m_group, pitch);
}

float SoundGroup::getPan() const
{
    return ma_sound_group_get_pan(&m_group);
}

void SoundGroup::setPan(float pan)
{
    return ma_sound_group_set_pan(&m_group, pan);
}

float SoundGroup::getRollOff() const
{
    return ma_sound_group_get_rolloff(&m_group);
}

void SoundGroup::setRolloff(float rolloff)
{
    ma_sound_group_set_rolloff(&m_group, rolloff);
}

void SoundGroup::getCone(float &innerAngle, float &outerAngle, float &outerGrain)
{
    ma_sound_group_get_cone(&m_group, &innerAngle, &outerAngle, &outerGrain);
}

void SoundGroup::setCone(float innerAngle, float outerAngle, float outerGrain)
{
    ma_sound_group_set_cone(&m_group, innerAngle, outerAngle, outerGrain);
}

float SoundGroup::getMinDistance() const
{
    return ma_sound_group_get_min_distance(&m_group);
}

void SoundGroup::setMinDistance(float min)
{
    ma_sound_group_set_min_distance(&m_group, min);
}

float SoundGroup::getMaxDistance() const
{
    return ma_sound_group_get_max_distance(&m_group);
}

void SoundGroup::setMaxDistance(float max)
{
    ma_sound_group_set_max_distance(&m_group, max);
}

float SoundGroup::getDopplerFactor() const
{
    return ma_sound_group_get_doppler_factor(&m_group);
}

void SoundGroup::setDopplerFactor(float value)
{
    ma_sound_group_set_doppler_factor(&m_group, value);
}

float SoundGroup::getMinGains() const
{
    return ma_sound_group_get_min_gain(&m_group);
}

void SoundGroup::setMinGains(float min)
{
    ma_sound_group_set_min_gain(&m_group, min);
}

float SoundGroup::getMaxGains() const
{
    return ma_sound_get_max_gain(&m_group);
}

void SoundGroup::setMaxGains(float max)
{
    ma_sound_set_max_gain(&m_group, max);
}

AttenuationMode::Type SoundGroup::getAttenuationMode() const
{
    return AttenuationMode::FromMA(ma_sound_group_get_attenuation_model(&m_group));
}

void SoundGroup::setAttenuationMode(AttenuationMode::Type type)
{
    ma_sound_group_set_attenuation_model(&m_group, AttenuationMode::ToMAModel(type));
}

PositioningMode::Type SoundGroup::getPositioningMode() const
{
    return PositioningMode::FromMA(ma_sound_group_get_positioning(&m_group));
}

void SoundGroup::setPositioningMode(PositioningMode::Type type)
{
    ma_sound_group_set_positioning(&m_group, PositioningMode::ToMA(type));
}

PanMode::Type SoundGroup::getPanMode() const
{
    return PanMode::FromMA(ma_sound_group_get_pan_mode(&m_group));
}

void SoundGroup::setPanMode(PanMode::Type type)
{
    ma_sound_group_set_pan_mode(&m_group, PanMode::ToMA(type));
}

ma_sound_group* SoundGroup::group()
{
    return &m_group;
}

AZ::Name SoundGroup::name() const
{
    return m_name;
}

SoundGroup *SoundGroup::parent() const
{
    return m_parent;
}

AZStd::vector<AZStd::unique_ptr<SoundGroup>> &SoundGroup::children()
{
    return m_children;
}

void SoundGroup::setName(const AZ::Name &newName)
{
    m_name = newName;
}


AZ::Name SoundGroupManager::EmptySoundGroup = AZ::Name();

void SoundGroupManager::ResetSoundGroupLayout()
{
    Reset();
    SoundGroupLayoutData groupLayout;
    GatherSoundGroupLayout(groupLayout);

    SoundGroupManagerNotificationBus::QueueBroadcast(&SoundGroupManagerNotificationBus::Events::OnUpdateSoundGroupLayout, groupLayout);
}

void SoundGroupManager::RequestSoundGroupData(AZ::Name name)
{
    SoundGroupData data;
    SoundGroup* group = GetSoundGroup(name);
    if(group)
    {
        group->GatherData(data);
        SoundGroupManagerNotificationBus::QueueBroadcast(&SoundGroupManagerNotificationBus::Events::OnUpdateSoundGroup, true, data);
    }
    else
    {
        SoundGroupManagerNotificationBus::QueueBroadcast(&SoundGroupManagerNotificationBus::Events::OnUpdateSoundGroup, false, data);
    }
}

void SoundGroupManager::LoadSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath)
{
    LoadSoundGroupsLayout(layoutFilePath);
    SoundGroupLayoutData layout;
    GatherSoundGroupLayout(layout);

    SoundGroupManagerNotificationBus::QueueBroadcast(&SoundGroupManagerNotificationBus::Events::OnUpdateSoundGroupLayout, layout);
}

void SoundGroupManager::SaveSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath)
{
    SaveSoundGroupsLayout(layoutFilePath);
}

void SoundGroupManager::RequestSoundGroupLayout()
{
    SoundGroupLayoutData layout;
    GatherSoundGroupLayout(layout);
    SoundGroupManagerNotificationBus::QueueBroadcast(&SoundGroupManagerNotificationBus::Events::OnUpdateSoundGroupLayout, layout);
}



SoundGroupManager::SoundGroupManager(ma_engine *engine)
    : m_engine(engine)
{
    SoundGroupManagerRequestBus::Handler::BusConnect();
}

SoundGroupManager::~SoundGroupManager()
{
    SoundGroupManagerRequestBus::Handler::BusDisconnect();
}

void SoundGroupManager::Reset()
{
    m_soundGroups.clear();
    m_soundGroupsFlatMap.clear();
}

bool SoundGroupManager::LoadSoundGroupsLayout(AZ::IO::PathView layoutFilePath)
{
    SoundGroupLayoutData layout;
    if(!layout.Load(layoutFilePath))
    {
        return false;
    }

    Reset();

    for(auto& groupData : layout.m_groups)
    {
        SoundGroup* group = CreateSoundGroup(groupData.m_groupName, EmptySoundGroup);
        if(!group)
        {
            Reset();
            return false;
        }

        group->ApplyData(&groupData);
        PopulateChildSoundGroups(group, groupData.m_children);
    }

    return true;
}

bool SoundGroupManager::SaveSoundGroupsLayout(AZ::IO::PathView layoutFilePath)
{
    SoundGroupLayoutData layout;
    GatherSoundGroupLayout(layout);
    return layout.Save(layoutFilePath);
}

SoundGroup *SoundGroupManager::CreateSoundGroup(AZ::Name &name, AZ::Name &parentName)
{
    if(IsSoundGroupExists(name))
    {
        AZ_Warning(Constants::LogWindow, false, "Trying to create sound group named '%s' already exists!", name.GetCStr());
        return nullptr;
    }

    auto parent = GetSoundGroup(parentName);
    auto newGroup = AZStd::make_unique<SoundGroup>(m_engine, name, parent, nullptr);
    if(!newGroup)
    {
        return nullptr;
    }

    SoundGroup* newPtr = nullptr;
    if(parent)
    {
        parent->children().push_back(AZStd::move(newGroup));
        newPtr = parent->children().back().get();
    }
    else
    {
        m_soundGroups.push_back(AZStd::move(newGroup));
        newPtr = m_soundGroups.back().get();
    }

    m_soundGroupsFlatMap[name] = newPtr;
    return newPtr;
}

SoundGroup *SoundGroupManager::CreateSoundGroup(AZ::Name &name, SoundGroup *parent)
{
    auto newGroup = AZStd::make_unique<SoundGroup>(m_engine, name, parent, nullptr);
    if(!newGroup)
    {
        return nullptr;
    }

    SoundGroup* newPtr = nullptr;
    if(parent)
    {
        parent->children().push_back(AZStd::move(newGroup));
        newPtr = parent->children().back().get();
    }
    else
    {
        m_soundGroups.push_back(AZStd::move(newGroup));
        newPtr = m_soundGroups.back().get();
    }

    m_soundGroupsFlatMap[name] = newPtr;
    return newPtr;
}

bool SoundGroupManager::DestroySoundGroup(AZ::Name &name)
{
    if(!IsSoundGroupExists(name))
    {
        return false;
    }

    SoundGroup* group = GetSoundGroup(name);
    SoundGroup* parent = group->parent();
    auto erasePredicate = [group](auto& ptr) { return ptr.get() == group; };
    bool deleted = false;

    if(parent)
    {
        auto it = AZStd::find_if(parent->m_children.begin(), parent->m_children.end(), erasePredicate);
        if(it != parent->m_children.end())
        {
            parent->m_children.erase(it);
            deleted = true;
        }
    }
    else
    {
        auto it = AZStd::find_if(m_soundGroups.begin(), m_soundGroups.end(), erasePredicate);
        if(it != m_soundGroups.end())
        {
            m_soundGroups.erase(it);
            deleted = true;
        }
    }

    if(deleted)
    {
        m_soundGroupsFlatMap.erase(name);
        return true;
    }

    return false;
}

bool SoundGroupManager::RenameSoundGroup(AZ::Name &oldname, AZ::Name newName)
{
    if(!IsSoundGroupExists(oldname))
        return false;

    if(oldname == newName)
        return true;

    SoundGroup* grpNewMatch = GetSoundGroup(newName);
    if(grpNewMatch)
        return false;

    SoundGroup* group = GetSoundGroup(oldname);
    if(!group)
        return false;

    auto grpHandle = m_soundGroupsFlatMap.extract(oldname);
    if(grpHandle.empty())
        return false;

    grpHandle.key() = newName;
    m_soundGroupsFlatMap.insert(AZStd::move(grpHandle));
    m_soundGroupsFlatMap[newName]->setName(newName);

    return true;
}

bool SoundGroupManager::IsSoundGroupExists(const AZ::Name &name)
{
    if(name.IsEmpty())
        return false;

    return m_soundGroupsFlatMap.find(name) != m_soundGroupsFlatMap.end();
}

SoundGroup *SoundGroupManager::GetSoundGroup(const AZ::Name &name) const
{
    if(name.IsEmpty())
        return nullptr;

    auto match = m_soundGroupsFlatMap.find(name);
    if(match != m_soundGroupsFlatMap.end())
    {
        return match->second;
    }

    return nullptr;
}


AZStd::vector<AZ::Name> SoundGroupManager::GetSoundGroupEntries() const
{
    AZStd::vector<AZ::Name> entries;


    for(auto& group : m_soundGroupsFlatMap)
    {
        entries.push_back(group.first);
    }

    return entries;
}

bool SoundGroupManager::PopulateChildSoundGroups(SoundGroup* parent, AZStd::vector<SoundGroupData> &childgroups)
{
    for(auto& data : childgroups)
    {
        SoundGroup* child = CreateSoundGroup(data.m_groupName, parent);
        if(!child)
        {
            return false;
        }
        child->ApplyData(&data);
        PopulateChildSoundGroups(child, data.m_children);
    }

    return true;
}

void SoundGroupManager::RequestSoundGroupEntries()
{
    SoundGroupManagerNotificationBus::QueueBroadcast(&SoundGroupManagerNotificationBus::Events::OnUpdateSoundGroupEntries, GetSoundGroupEntries());
}

void SoundGroupManager::GatherSoundGroupLayout(SoundGroupLayoutData &layout) const
{
    layout.m_groups.clear();
    layout.m_groups.reserve(m_soundGroups.size());

    for(auto& groupPtr : m_soundGroups)
    {
        SoundGroupData data;
        groupPtr->GatherData(data);
        GatherChildSoundGroupData(groupPtr.get(), data);
        layout.m_groups.emplace_back(data);
    }
}

void SoundGroupManager::GatherChildSoundGroupData(SoundGroup *groupPtr, SoundGroupData &data) const
{
    data.m_children.reserve(groupPtr->m_children.size());
    for(auto& childPtr : groupPtr->m_children)
    {
        SoundGroupData childData;
        childPtr->GatherData(childData);
        GatherChildSoundGroupData(childPtr.get(), childData);

        data.m_children.emplace_back(childData);
    }
}

void SoundGroupManager::AddSoundGroup(AZ::Name name, AZ::Name parentName)
{
    SoundGroup* group = CreateSoundGroup(name, parentName);
    SoundGroupData  data;

    if(group)
    {
        AZ_Info(Constants::LogWindow, "Created Sound Group '%s'!", group->m_name.GetCStr());
        group->GatherData(data);
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_AddSoundGroup,
                group != nullptr, data,
                group->parent() ? group->m_parent->m_name : AZ::Name()
                );
}

void SoundGroupManager::RemoveSoundGroup(AZ::Name name)
{
    bool success = DestroySoundGroup(name);
    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_RemoveSoundGroup, success, name);
}

void SoundGroupManager::ChangeSoundGroupName(AZ::Name oldname, AZ::Name newName)
{
    bool success = RenameSoundGroup(oldname, newName);
    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_RenameSoundGroup,
                success, oldname, newName);
}

void SoundGroupManager::SetSoundGroupSpatializationEnabled(AZ::Name name, bool value)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);
    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the spatialization of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setSpatializationEnabled(value);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSpatializationEnabled, success, name, value);

}

void SoundGroupManager::SetSoundGroupVolume(AZ::Name name, float volume)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);
    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the volume of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setVolume(volume);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupVolume, success, name, volume);
}

void SoundGroupManager::SetSoundGroupPitch(AZ::Name name, float pitch)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the pitch of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setPitch(pitch);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupPitch, success, name, pitch);
}

void SoundGroupManager::SetSoundGroupPan(AZ::Name name, float pan)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the pan of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setPan(pan);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupPan, success, name, pan);

}

void SoundGroupManager::SetSoundGroupRolloff(AZ::Name name, float rolloff)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the rolloff of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setRolloff(rolloff);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupRolloff, success, name, rolloff);

}

void SoundGroupManager::SetSoundGroupMinDistance(AZ::Name name, float minDistance)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the min distance of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setMinDistance(minDistance);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupMinDistance, success, name, minDistance);

}

void SoundGroupManager::SetSoundGroupMaxDistance(AZ::Name name, float maxDistance)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the max distance of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setMaxDistance(maxDistance);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupMaxDistance, success, name, maxDistance);

}

void SoundGroupManager::SetSoundGroupMinGains(AZ::Name name, float minGains)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the min gains of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setMinDistance(minGains);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupMinGains, success, name, minGains);


}

void SoundGroupManager::SetSoundGroupMaxGains(AZ::Name name, float maxGains)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the max gains of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setMaxGains(maxGains);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupMaxGains, success, name, maxGains);

}

void SoundGroupManager::SetSoundGroupAttenuationMode(AZ::Name name, AttenuationMode::Type type)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the attenuation mode of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setAttenuationMode(type);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupAttenuationMode, success, name, type);

}

void SoundGroupManager::SetSoundGroupPositioning(AZ::Name name, PositioningMode::Type type)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the positioning mode of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setPositioningMode(type);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupPositioning, success, name, type);

}

void SoundGroupManager::SetSoundGroupPanMode(AZ::Name name, PanMode::Type type)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the pan mode of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setPanMode(type);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupPan, success, name, type);

}

void SoundGroupManager::SetSoundGroupDopplerFactor(AZ::Name name, float factor)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the doppler factor of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setMaxGains(factor);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupDopplerFactor, success, name, factor);

}

void SoundGroupManager::SetSoundGroupAttenuationCone(AZ::Name name, float innerAngle, float outerAngle, float outerGrain)
{
    bool success = false;
    SoundGroup* group = GetSoundGroup(name);

    if(!group)
    {
        AZ_Error(Constants::LogWindow, false, "unable to change the doppler factor of sound group '%s' because it doesn't exist",
                 name.GetCStr());
    }
    else
    {
        group->setCone(innerAngle, outerAngle, outerGrain);
        success = true;
    }

    SoundGroupManagerNotificationBus::QueueBroadcast(
                &SoundGroupManagerNotificationBus::Events::OnRequestCompleted_SetSoundGroupAttenuationCone, success, name, innerAngle, outerAngle, outerGrain);

}

}