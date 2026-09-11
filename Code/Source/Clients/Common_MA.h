#pragma once

namespace AudioEngineMA
{
    namespace Constants {
        constexpr const char* LogWindow = "AudioEngine - MiniAudio";
        constexpr const char* MasterGroupName = "Master";
    }

    namespace MAXMLTags {
        static constexpr const char* AudioFileTag = "AudioFile";
        static constexpr const char* AudioFilePathAttr = "Path";
        static constexpr const char* AudioFileLocalizedAttr = "Localized";
    }
}