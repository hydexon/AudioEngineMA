#pragma once

#include <AudioAllocators.h>
#include <AzCore/Name/Name.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/IO/Path/Path.h>

//#include "../EditorEngineInterop.h"
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

    //SoundGroup(ma_engine* engine, AZ::Name& name, bool noSpatial, bool noPitch, SoundGroup* parent = nullptr);
    //~SoundGroup();

    AZ::Name& GetName() { return m_groupName; }
    bool SetName(AZ::Name& name);


private:
    ma_engine* m_engine;
    ma_sound_group* m_group;
    AZ::Name m_groupName;
    SoundGroup* m_parent;
    AZStd::vector<SoundGroup> m_children;

    //Common Data:
    float m_volume = 1.0f;
    float m_pitch  = 1.0f;
    float m_pan    = 1.0f;
    float m_rolloff = 1.0f;
    float m_coneinnerAngle = (2 * 3.14159265358979323846264f);
    float m_coneOuterAngle = (2 * 3.14159265358979323846264f);
    float m_coneOuterGrain = 1.0f;
    float m_minDistance = 1.0f;
    float m_maxDistance = FLT_MAX;
    float m_dopplerFactor = 1.0f;
    float m_minGains = 0.1f;
    float m_maxGains = 1.0f;
    bool m_spatializationEnable = true;
    AttenuationMode::Type m_attenuationMode = AttenuationMode::Inverse;
    PositioningMode::Type m_positioningType = PositioningMode::Absolute;
    PanMode::Type m_panMode = PanMode::Balance;
};

#if 0
class SoundGroupManager : private SoundGroupManagerRequestBus::Handler
{
public:    
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroupManager);
    AZ_DISABLE_COPY_MOVE(SoundGroupManager);

    SoundGroup* CreateSoundGroup(AZ::Name& name, bool noSpatial, bool noPitch, AZ::Name& parentName);
    bool DestroySoundGroup(AZ::Name& name);

    bool IsSoundGroupExists(const AZ::Name& name);
    SoundGroup* GetSoundGroup(const AZ::Name& name) const;

private:
    AZStd::vector<AZStd::unique_ptr<SoundGroup>> m_soundGroups;
    AZStd::unordered_map<AZ::Name, SoundGroup*>  m_soundGroupsFlatMap;

    void AddSoundGroup(AZ::Name name, bool noSpatial, bool noPitch,
                       AZ::Name parentName = AZ::Name()) override;

    void RemoveSoundGroup(AZ::Name name) override;

    void ChangeSoundGroupName(AZ::Name name) override;
};
#endif

}