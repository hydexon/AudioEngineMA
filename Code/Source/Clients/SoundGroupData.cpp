#include "SoundGroupData.h"
#include <AzCore/Serialization/SerializeContext.h>

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Serialization/Json/JsonSerialization.h>
#include <AzCore/Serialization/Json/JsonUtils.h>

#include "Common_MA.h"

namespace AudioEngineMA
{

void SoundGroupData::Reflect(AZ::ReflectContext* context)
{
    if(auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
        serializeContext->Class<SoundGroupData>()
                ->Version(1)
                ->Field("Name", &SoundGroupData::m_groupName)
                ->Field("m_spatializationEnabled", &SoundGroupData::m_spatialization)
                ->Field("Volume", &SoundGroupData::m_volume)
                ->Field("Pitch", &SoundGroupData::m_pitch)
                ->Field("Pan", &SoundGroupData::m_pan)
                ->Field("Rolloff", &SoundGroupData::m_rolloff)
                ->Field("ConeInnerAngle", &SoundGroupData::m_coneinnerAngle)
                ->Field("ConeOuterAngle", &SoundGroupData::m_coneOuterAngle)
                ->Field("ConeOuterGrain", &SoundGroupData::m_coneOuterGrain)
                ->Field("MinDistance", &SoundGroupData::m_minDistance)
                ->Field("MaxDistance", &SoundGroupData::m_maxDistance)
                ->Field("DopplerFactor", &SoundGroupData::m_dopplerFactor)
                ->Field("MinGains", &SoundGroupData::m_minGains)
                ->Field("MaxGains", &SoundGroupData::m_maxGains)
                ->Field("AttenuationMode", &SoundGroupData::m_attenuationMode) //Raw integer value fallback, should consider O3DE RTTI enums?
                ->Field("PositioningType", &SoundGroupData::m_positioningType)
                ->Field("PanMode", &SoundGroupData::m_panMode)
                ->Field("Children", &SoundGroupData::m_children);
    }
}

void SoundGroupData::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{

}

void SoundGroupData::WriteToXML(AZ::rapidxml::xml_node<char> &node, AZ::rapidxml::memory_pool<> &xmlAlloc) const
{
}

void SoundGroupLayoutData::Reflect(AZ::ReflectContext* context)
{
    if(auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
        serializeContext->Class<SoundGroupLayoutData>()
                ->Version(1)
                ->Field("Groups", &SoundGroupLayoutData::m_groups);
    }
}

bool SoundGroupLayoutData::Save(AZ::IO::PathView filePath)
{
    AZ::JsonSerializerSettings serializerSettings;
    rapidjson::Document jsonDoc;

    auto result = AZ::JsonSerialization::Store(jsonDoc, jsonDoc.GetAllocator(), *this, serializerSettings);
    if(result.GetProcessing() != AZ::JsonSerializationResult::Processing::Completed)
    {
        AZ_Error(Constants::LogWindow, false, "Unable to serialize json file '%s'", filePath.Native().data());
        return false;
    }

    auto outcome = AZ::JsonSerializationUtils::WriteJsonFile(jsonDoc, filePath.Native());
    if(!outcome)
    {
        AZ_Error(Constants::LogWindow, false, "%s", outcome.GetError().c_str());
        return false;
    }

    return true;
}

bool SoundGroupLayoutData::Load(AZ::IO::PathView filePath)
{
    auto outcome = AZ::JsonSerializationUtils::ReadJsonFile(filePath.Native());
    if (!outcome)
    {
        AZ_Error(Constants::LogWindow, false, "%s", outcome.GetError().c_str());
        return false;
    }

    AZ::JsonDeserializerSettings deserializeSettings;
    AZ::ComponentApplicationBus::BroadcastResult(
        deserializeSettings.m_serializeContext, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

    auto result = AZ::JsonSerialization::Load(*this, outcome.GetValue(), deserializeSettings);
    if (result.GetProcessing() != AZ::JsonSerializationResult::Processing::Completed)
    {
        AZ_Error(Constants::LogWindow, false, "Unable to deserialize json file '%s'", filePath.Native().data());
        return false;
    }

    return true;
}

void SoundGroupLayoutData::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{

}

void SoundGroupLayoutData::WriteToXML(AZ::rapidxml::xml_node<char> &node, AZ::rapidxml::memory_pool<> &xmlAlloc) const
{
}

}