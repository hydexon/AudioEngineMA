#pragma once

#include <AzCore/Name/Name.h>
#include <AzCore/XML/rapidxml.h>
#include <AzCore/RTTI/TypeInfo.h>

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
    }

    struct AudioFileTriggerParameters
    {
        AudioFileTriggerParameters() = default;
        ~AudioFileTriggerParameters() = default;

        void ReadFromXML(const AZ::rapidxml::xml_node<char>& node);
        void WriteToXML(AZ::rapidxml::xml_node<char>& node, AZ::rapidxml::memory_pool<>& xmlAlloc) const;

        constexpr static const char* TagTriggerParams = "TriggerParams";

        AZ::Name m_soundGroupName = AZ::Name();
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

    namespace ATLData
    {
        void ReflectEnums(AZ::ReflectContext*  context);
    }
}

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(AudioEngineMA::SoundAction::Type, "{09120DF3-53FF-4C18-A058-A463E8317C4C}");
    AZ_TYPE_INFO_SPECIALIZE(AudioEngineMA::AttenuationMode::Type, "{0F6107B5-E220-4AA7-9DB2-DF74EA2A9F19}");
    AZ_TYPE_INFO_SPECIALIZE(AudioEngineMA::PositioningMode::Type, "{E6D53F75-10C4-4EFA-8609-A02F5873599F}");
    AZ_TYPE_INFO_SPECIALIZE(AudioEngineMA::PanMode::Type, "{971285F7-D018-4DA3-83F4-BAE1531C6860}");
}