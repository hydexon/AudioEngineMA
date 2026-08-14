
#include "AudioEngineMAModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <AudioEngineMA/AudioEngineMATypeIds.h>

#include <Clients/AudioEngineMASystemComponent.h>

namespace AudioEngineMA
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(AudioEngineMAModuleInterface,
        "AudioEngineMAModuleInterface", AudioEngineMAModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(AudioEngineMAModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(AudioEngineMAModuleInterface, AZ::SystemAllocator);

    AudioEngineMAModuleInterface::AudioEngineMAModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            AudioEngineMASystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList AudioEngineMAModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<AudioEngineMASystemComponent>(),
        };
    }
} // namespace AudioEngineMA
