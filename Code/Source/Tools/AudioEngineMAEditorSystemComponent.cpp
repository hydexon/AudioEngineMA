
#include <AzCore/Serialization/SerializeContext.h>
#include "AudioEngineMAEditorSystemComponent.h"

#include <AudioEngineMA/AudioEngineMATypeIds.h>
#include "AudioSystemEditor_MA.h"

namespace AudioEngineMA
{
    AZ_COMPONENT_IMPL(AudioEngineMAEditorSystemComponent, "AudioEngineMAEditorSystemComponent",
        AudioEngineMAEditorSystemComponentTypeId, BaseSystemComponent);

    void AudioEngineMAEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AudioEngineMAEditorSystemComponent, AudioEngineMASystemComponent>()
                ->Version(0);
        }
    }

    AudioEngineMAEditorSystemComponent::AudioEngineMAEditorSystemComponent() = default;

    AudioEngineMAEditorSystemComponent::~AudioEngineMAEditorSystemComponent() = default;

    void AudioEngineMAEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
    }

    void AudioEngineMAEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
    }

    void AudioEngineMAEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void AudioEngineMAEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void AudioEngineMAEditorSystemComponent::InitializeEditorImplPlugin()
    {
        m_editorImplPlugin.reset(new CAudioSystemEditor_MiniAudio());
    }

    void AudioEngineMAEditorSystemComponent::ReleaseEditorImplPlugin()
    {
        m_editorImplPlugin.release();
    }

    AudioControls::IAudioSystemEditor *AudioEngineMAEditorSystemComponent::GetEditorImplPlugin()
    {
        return m_editorImplPlugin.get();
    }

    void AudioEngineMAEditorSystemComponent::Activate()
    {
        AudioEngineMASystemComponent::Activate();
        AudioControlsEditor::EditorImplPluginEventBus::Handler::BusConnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void AudioEngineMAEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AudioControlsEditor::EditorImplPluginEventBus::Handler::BusDisconnect();
        AudioEngineMASystemComponent::Deactivate();
    }

} // namespace AudioEngineMA
