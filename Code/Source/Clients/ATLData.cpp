#include "ATLData.h"

namespace AudioEngineMA
{

const char *SoundAction::ToString(Type type)
{
    static const char* strings[SoundAction::Count] = { "Start", "Stop", "Pause", "Resume" };

    return strings[type];
}

SoundAction::Type SoundAction::FromString(const char *str)
{

}


} // namespace AudioEngineMA