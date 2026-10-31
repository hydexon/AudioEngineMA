#pragma once

#include <AzCore/Name/Name.h>
#include <AzCore/XML/rapidxml.h>

#include "Common_MA.h"
#include <miniaudio.h>
#include <float.h>

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
        ma_attenuation_model ToMAModel(Type type);
    }

    namespace PanMode
    {
        enum Type
        {
            Balance,
            Pan,
            Count
        };

        constexpr static const char* Tag = "PanMode";

        const char* ToString(Type type);
        Type FromString(const char* str);
        ma_pan_mode ToMA(Type type);
    }

    namespace PositioningMode
    {
        enum Type
        {
            Absolute,
            Relative,
            Count
        };

        constexpr static const char* Tag = "PositioningMode";

        const char* ToString(Type type);
        Type FromString(const char* str);
        ma_positioning ToMA(Type type);
    }

    struct AudioFileTriggerParameters
    {
        AudioFileTriggerParameters() = default;
        ~AudioFileTriggerParameters() = default;

        void ReadFromXML(const AZ::rapidxml::xml_node<char>& node);
        void WriteToXML(AZ::rapidxml::xml_node<char>& node, AZ::rapidxml::memory_pool<>& xmlAlloc) const;

        constexpr static const char* TagTriggerParams = "TriggerParams";
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

        AZ::Name m_soundGroupName = AZ::Name(Constants::MasterGroupName);
        SoundAction::Type m_soundAction = SoundAction::Type::Start;
        AttenuationMode::Type m_attenuationMode = AttenuationMode::Type::Inverse;
        PanMode::Type m_panMode = PanMode::Balance;
        PositioningMode::Type m_positioning = PositioningMode::Absolute;
        float m_rolloff = 1.0f;
        float m_pitch = 1.0f;
        float m_pan = 0.0f;
        float m_coneinnerAngle = (2 * 3.14159265358979323846264f);
        float m_coneOuterAngle = (2 * 3.14159265358979323846264f);
        float m_coneOuterGrain = 1.0f;
        float m_minDistance = 1.0f;
        float m_maxDistance = FLT_MAX;
        float m_dopplerFactor = 1.0f;
        float m_volume = 1.0f;
        float m_minGains = 0.1f;
        float m_maxGains = 1.0f;
        bool m_looping = false;
        bool m_spatialization = true;
    };

    namespace AudioFileRTPC
    {
        enum Type
        {
            Volume,
            Pitch,
            Count
        };

        constexpr static const char* Tag = "AudioFileRTPCType";

        const char* ToString(Type type);
        Type FromString(const char* str);
    }

    struct AudioFileRTPCParams
    {
        AudioFileRTPCParams() = default;
        ~AudioFileRTPCParams() = default;

        constexpr static const char* TagPerObject = "PerObject";
        constexpr static const char* TagMult = "Mult";
        constexpr static const char* TagShift = "Shift";

        bool ReadFromXML(const AZ::rapidxml::xml_node<char>& node);
        void WriteToXML(AZ::rapidxml::xml_node<char>& node, AZ::rapidxml::memory_pool<>& xmlAlloc) const;

        AudioFileRTPC::Type m_type = AudioFileRTPC::Volume;
        bool m_perObject = false;
        float m_mult = 1.0f;
        float m_shift = 0.0f;
    };

    namespace GlobalRTPC
    {
        enum Type
        {
            GlobalVolume,
            Count
        };

        constexpr static const char* Tag = "GlobalRTPC";

        const char* ToString(Type type);
        Type FromString(const char* str);
    }

    struct GlobalRTPCParams
    {
        GlobalRTPCParams()  = default;
        ~GlobalRTPCParams() = default;

        constexpr static const char* TagMult = "Mult";
        constexpr static const char* TagShift = "Shift";

        bool ReadFromXML(const AZ::rapidxml::xml_node<char>& node);
        void WriteToXML(AZ::rapidxml::xml_node<char>& node, AZ::rapidxml::memory_pool<>& xmlAlloc) const;

        GlobalRTPC::Type m_type = GlobalRTPC::GlobalVolume;
        float m_mult  = 1.0f;
        float m_shift = 0.0f;
    };

}