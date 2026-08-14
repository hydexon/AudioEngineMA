#pragma once

#include <AzCore/std/string/string.h>
#include <AzCore/IO/Path/Path.h>

namespace AudioEngineMA
{
    namespace Config
    {
        constexpr const auto AudioFilesPath = AZ::IO::FixedMaxPath("Assets/Audio/MiniAudio").LexicallyNormal();
        constexpr const char* LocalizationDirName = "localization";
        constexpr const char* SupportedFileFormatsArray[] = {"wav", "mp3", "ogg", "flac"};
        constexpr const char* MANodeLayoutExt = ".ma_nodegraph";
    }
}