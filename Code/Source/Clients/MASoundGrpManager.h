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

class SoundGroup
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroup);
    AZ_DISABLE_COPY_MOVE(SoundGroup);

    SoundGroup(SoundGroupManager& manager, ma_engine* engine, ma_sound_group* parent, const AZ::Name& name);
private:
    SoundGroupManager& m_grpManager;
    AZ::Name m_name;
    ma_sound_group* m_group;
};

class SoundGroupManager
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(SoundGroupManager);
    AZ_DISABLE_COPY_MOVE(SoundGroupManager);

    SoundGroupManager(ma_engine* engine);
    ~SoundGroupManager();

    void Reset();

    SoundGroup *CreateSoundGroup(const AZ::Name& grpName);
    bool DestroyGroup(const AZ::Name& grpName);
    bool DestroyGroup(int32_t grpIndex);

    bool LoadGroupDefinitions(AZ::IO::PathView definitionFilePath);
    bool SaveGroupDefinitions(AZ::IO::PathView definitionFilePath);


private:
    ma_engine* m_engine;
    AZStd::vector<AZStd::unique_ptr<SoundGroup>> m_soundGroups;
};

}