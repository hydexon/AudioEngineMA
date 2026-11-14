#include "AudioEngineMA_CVars.h"

namespace AudioEngineMA::CVars
{
    AZ_CVAR(float,
        s_MiniAudio_MinLowPassCutoffSize,
        400.0f,
        nullptr,
        AZ::ConsoleFunctorFlags::Null,
        "Sets the minimum LPF cutoff frequency upon full audio object occlusion.\n"
        "Usage: s_MiniAudio_MinLowPassCutoffSize [400.0/...]\n"
        "Default: 400.0");

    AZ_CVAR(float,
        s_MiniAudio_MaxLowPassCutoffSize,
        20000.0f,
        nullptr,
        AZ::ConsoleFunctorFlags::Null,
        "Sets the maximum LPF cutoff frequency upon full audio object occlusion\n"
        "Usage: s_MiniAudio_MaxLowPassCutoffSize [20000.0/...]\n"
        "Default: 20000.0");

    AZ_CVAR(bool,
        s_MiniAudio_DisableLibVorbisDecoding,
        false,
        nullptr,
        AZ::ConsoleFunctorFlags::NeedsReload,
        "Disables/Enables use of libvorbis for .ogg files and use the miniaudio included stb_vorbis codec instead\n"
        "Usage: s_MiniAudio_DisableLibVorbisDecoding false\n"
        "Default: false");

}