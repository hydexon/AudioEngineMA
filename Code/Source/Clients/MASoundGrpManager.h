#pragma once

#include <AudioAllocators.h>
#include <AzCore/Name/Name.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

#include <miniaudio.h>

namespace AudioEngineMA {

class MASoundGrpManager;

class MASoundGroup
{
public:
    AUDIO_IMPL_CLASS_ALLOCATOR(MASoundGroup);
    AZ_DISABLE_COPY_MOVE(MASoundGroup);

    //MASoundGroup(MASoundGrpManager& manager, ma_engine* engine, const AZ::Name& name);
private:
    /*
    MASoundGrpManager& m_grpManager;
    AZ::Name m_name;
    ma_sound_group* m_group;*/
};

class MASoundGrpManager
{
public:
};

}