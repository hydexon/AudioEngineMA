
#include <AudioEngineMA/AudioEngineMATypeIds.h>
#include <AudioEngineMAModuleInterface.h>
#include "AudioEngineMAEditorSystemComponent.h"

namespace AudioEngineMA
{
    class AudioEngineMAEditorModule
        : public AudioEngineMAModuleInterface
    {
    public:
        AZ_RTTI(AudioEngineMAEditorModule, AudioEngineMAEditorModuleTypeId, AudioEngineMAModuleInterface);
        AZ_CLASS_ALLOCATOR(AudioEngineMAEditorModule, AZ::SystemAllocator);

        AudioEngineMAEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                AudioEngineMAEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<AudioEngineMAEditorSystemComponent>(),
            };
        }
    };
}// namespace AudioEngineMA

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), AudioEngineMA::AudioEngineMAEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_AudioEngineMA_Editor, AudioEngineMA::AudioEngineMAEditorModule)
#endif
