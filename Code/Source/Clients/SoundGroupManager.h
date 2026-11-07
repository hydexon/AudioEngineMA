#pragma once

#include <AudioAllocators.h>
#include <AzCore/Name/Name.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/IO/Path/Path.h>

#include "../EditorEngineInterop.h"
#include "SoundGroupData.h"
#include <miniaudio.h>

namespace AudioEngineMA
{

class SoundGroupManager;

class SoundGroup
{
public:
    AZ_DISABLE_COPY(SoundGroup);
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroup);
    friend class SoundGroupManager;

    SoundGroup(ma_engine* engine, AZ::Name& name, SoundGroup* parent = nullptr, SoundGroupData* srcdata = nullptr);
    ~SoundGroup();

    bool AddSound(ma_sound* sound);
    void RemoveSound(ma_sound* sound);

    void ApplyData(SoundGroupData* srcdata);
    void GatherData(SoundGroupData& dstData);

    bool isSpatializationEnabled();
    void setSpatializationEnabled(bool enabled);
    float getVolume() const;
    void setVolume(float volume);
    float getPitch() const;
    void setPitch(float pitch);
    float getPan() const;
    void setPan(float pan);
    float getRollOff() const;
    void setRolloff(float rolloff);
    void getCone(float& innerAngle, float& outerAngle, float& outerGrain);
    void setCone(float innerAngle, float outerAngle, float outerGrain);
    float getMinDistance() const;
    void setMinDistance(float min);
    float getMaxDistance() const;
    void setMaxDistance(float max);
    float getDopplerFactor() const;
    void setDopplerFactor(float value);
    float getMinGains() const;
    void setMinGains(float min);
    float getMaxGains() const;
    void setMaxGains(float max);
    AttenuationMode::Type getAttenuationMode() const;
    void setAttenuationMode(AttenuationMode::Type type);
    PositioningMode::Type getPositioningMode() const;
    void setPositioningMode(PositioningMode::Type type);
    PanMode::Type getPanMode() const;
    void setPanMode(PanMode::Type type);

    ma_sound_group *group();
    AZ::Name name() const;
    SoundGroup *parent() const;
    AZStd::vector<AZStd::unique_ptr<SoundGroup>>& children();

    void setName(const AZ::Name &newName);

private:
    ma_engine* m_engine;
    ma_sound_group m_group;
    AZ::Name m_name;
    SoundGroup* m_parent;
    AZStd::vector<AZStd::unique_ptr<SoundGroup>> m_children;
    AZStd::vector<ma_sound*> m_associatedSounds;
};

class SoundGroupManager : private SoundGroupManagerRequestBus::Handler
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroupManager);
    AZ_DISABLE_COPY_MOVE(SoundGroupManager);

    static AZ::Name EmptySoundGroup;
    explicit SoundGroupManager(ma_engine* engine);
    ~SoundGroupManager();

    void Reset();

    bool LoadSoundGroupsLayout(AZ::IO::PathView layoutFilePath);
    bool SaveSoundGroupsLayout(AZ::IO::PathView layoutFilePath);

    SoundGroup* CreateSoundGroup(AZ::Name& name, AZ::Name& parentName);
    SoundGroup* CreateSoundGroup(AZ::Name& name, SoundGroup* parent);

    bool DestroySoundGroup(AZ::Name& name);
    bool RenameSoundGroup(AZ::Name& oldname, AZ::Name newName );

    bool IsSoundGroupExists(const AZ::Name& name);
    SoundGroup* GetSoundGroup(const AZ::Name& name) const;

private:
    ma_engine* m_engine;
    AZStd::vector<AZStd::unique_ptr<SoundGroup>> m_soundGroups;
    AZStd::unordered_map<AZ::Name, SoundGroup*>  m_soundGroupsFlatMap;

    void GatherSoundGroupLayout(SoundGroupLayoutData& layout) const;
    void GatherChildSoundGroupData(SoundGroup *groupPtr, SoundGroupData& data) const;

    bool PopulateChildSoundGroups(SoundGroup *parent, AZStd::vector<SoundGroupData>& childgroups);

    //SoundGroupManagerRequests
    void RequestSoundGroupLayout() override;

    void ResetSoundGroupLayout() override;

    void LoadSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) override;

    void SaveSoundGroupLayout(AZ::IO::FixedMaxPath layoutFilePath) override;

    void AddSoundGroup(AZ::Name name,
                       AZ::Name parentName = AZ::Name()) override;

    void RemoveSoundGroup(AZ::Name name) override;

    void ChangeSoundGroupName(AZ::Name oldname, AZ::Name newName) override;

    void SetSoundGroupVolume(AZ::Name name, float volume) override;

    void SetSoundGroupPitch(AZ::Name name, float pitch) override;

    void SetSoundGroupPan(AZ::Name name, float pan) override;

    void SetSoundGroupRolloff(AZ::Name name, float rolloff) override;

    void SetSoundGroupMinDistance(AZ::Name name, float minDistance) override;

    void SetSoundGroupMaxDistance(AZ::Name name, float maxDistance) override;

    void SetSoundGroupMinGains(AZ::Name name, float minGains) override;

    void SetSoundGroupMaxGains(AZ::Name name, float maxGains) override;

    void SetSoundGroupAttenuationMode(AZ::Name name,
                                      AttenuationMode::Type type) override;

    void SetSoundGroupPositioning(AZ::Name name,
                                  PositioningMode::Type type) override;

    void SetSoundGroupPanMode(AZ::Name name, PanMode::Type type) override;

    void SetSoundGroupDopplerFactor(AZ::Name name, float factor) override;

    void SetSoundGroupAttenuationCone(AZ::Name name, float innerAngle,
                                      float outerAngle,
                                      float outerGrain) override;

};

}