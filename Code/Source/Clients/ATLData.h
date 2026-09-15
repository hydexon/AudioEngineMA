#pragma once

#include <AzCore/Name/Name.h>
#include <AzCore/XML/rapidxml.h>

#include "Common_MA.h"

namespace AudioEngineMA
{
    namespace SoundAction
    {
        enum Type
        {
            Start = 0,
            Stop,
            Pause,
            Resume,

            Count
        };

        constexpr static const char* Tag = "SoundAction";

        const char* ToString(Type type);
        SoundAction::Type FromString(const char* str);
    }

    namespace AttenuationMode
    {
        enum Type
        {
            None,
            Inverse,
            Linear,
            Exponential,

            Count
        };

        constexpr static const char* Tag = "AttenuationMode";

        const char* ToString(Type type);
        Type FromString(const char* str);
    }

    struct AudioFileTriggerParameters
    {
        AudioFileTriggerParameters() = default;
        ~AudioFileTriggerParameters() = default;

        void ReadFromXML(const AZ::rapidxml::xml_node<char>& node);
        void WriteToXML(AZ::rapidxml::xml_node<char>& node, AZ::rapidxml::memory_pool<>& xmlAlloc) const;

        constexpr static const char* TagGroupName   = "SoundGroupName";
        constexpr static const char* TagSoundAction = "SoundAction";
        constexpr static const char* TagAttenuationMode = "AttenuationMode";
        constexpr static const char* TagRolloff  = "Rolloff";
        constexpr static const char* TagDistances = "Distances"; //Takes m_minDistance, m_maxDistance
        constexpr static const char* TagCone = "AttenuationCone"; //Takes m_cone* variables.
        constexpr static const char* TagDopplerFactor = "DopplerFactor";
        constexpr static const char* TagLooping = "Looping";
        constexpr static const char* TagSpatialization = "Spatialization";

        AZ::Name m_soundGroupName = AZ::Name(Constants::MasterGroupName);
        SoundAction::Type m_soundAction = SoundAction::Type::Start;
        AttenuationMode::Type m_attenuationMode = AttenuationMode::Type::None;
        float m_rolloff = 0.0f;
        float m_coneinnerAngle = 0.0f;
        float m_coneOuterAngle = 0.0f;
        float m_coneOuterGrain = 0.0f;
        float m_minDistance = 3.0f;
        float m_maxDistance = 3.0f;
        float m_dopplerFactor = 0.0f;
        bool m_looping = false;
        bool m_spatialization = false;
    };

}