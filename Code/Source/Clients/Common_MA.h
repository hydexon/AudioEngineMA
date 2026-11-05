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

        //Audio and SoundGroup Tags:
        constexpr static const char* TagGroupName   = "SoundGroupName";
        constexpr static const char* TagSoundAction = "SoundAction";
        constexpr static const char* TagRolloff  = "Rolloff";
        constexpr static const char* TagPitch    = "Pitch";
        constexpr static const char* TagDistances = "Distances"; //Takes m_minDistance, m_maxDistance
        constexpr static const char* TagCone = "AttenuationCone"; //Takes m_cone* variables.
        constexpr static const char* TagDopplerFactor = "DopplerFactor";
        constexpr static const char* TagLooping = "Looping";
        constexpr static const char* TagSpatialization = "Spatialization";
        constexpr static const char* TagVolumeLinear = "Volume";
        constexpr static const char* TagGains = "Gains";
        constexpr static const char* TagPan = "Pan";
    }
}