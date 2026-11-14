#pragma once

#include <AzCore/Console/IConsole.h>

namespace AudioEngineMA::CVars
{
    AZ_CVAR_EXTERNED(float, s_MiniAudio_MinLowPassCutoffSize);
    AZ_CVAR_EXTERNED(float, s_MiniAudio_MaxLowPassCutoffSize);
    AZ_CVAR_EXTERNED(bool, s_MiniAudio_DisableLibVorbisDecoding);
}
