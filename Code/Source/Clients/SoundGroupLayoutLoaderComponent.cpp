#include "SoundGroupLayoutLoaderComponent.h"
#include <EditorEngineInterop.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AudioEngineMA
{
    void SoundGroupLayoutLoaderComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serialContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialContext->Class<SoundGroupLayoutLoaderComponent, AZ::Component>()
                ->Version(1)->Field("LayoutFilePath", &SoundGroupLayoutLoaderComponent::m_layoutFilePath);

            if (auto editContext = serialContext->GetEditContext())
            {
                editContext->Class<SoundGroupLayoutLoaderComponent>("MiniAudio Sound Group Layout Loader", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Audio")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
                    ->DataElement(AZ_CRC("LayoutFilePath"), &SoundGroupLayoutLoaderComponent::m_layoutFilePath, "Layout File", "");
            }
        }

    }

    void SoundGroupLayoutLoaderComponent::Activate()
    {
        if (!m_layoutFilePath.empty())
        {
            AZ::IO::FixedMaxPath path = m_layoutFilePath.c_str();
            SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::LoadSoundGroupLayout, path);
        }
    }

    void SoundGroupLayoutLoaderComponent::Deactivate()
    {
    }
}
