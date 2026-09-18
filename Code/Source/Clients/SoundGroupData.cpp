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
                ->Field("NoPitch", &SoundGroupData::m_noPitch)
                ->Field("NoSpatialization", &SoundGroupData::m_noSpatialization)
                ->Field("NoDefaultAttachment", &SoundGroupData::m_noDefaultAttachment)
                ->Field("Volume", &SoundGroupData::m_volume)
                ->Field("Pitch", &SoundGroupData::m_pitch)
                ->Field("Pan", &SoundGroupData::m_pan)
                ->Field("Children", &SoundGroupData::m_children);
    }
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

}