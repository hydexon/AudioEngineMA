
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/AudioEngineMASystemComponent.h>
#include <IAudioSystemEditor.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

namespace AudioEngineMA
{
    /// System component for AudioEngineMA editor
    class AudioEngineMAEditorSystemComponent
        : public AudioEngineMASystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
        , protected AudioControlsEditor::EditorImplPluginEventBus::Handler
    {
        using BaseSystemComponent = AudioEngineMASystemComponent;
    public:
        AZ_COMPONENT_DECL(AudioEngineMAEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        AudioEngineMAEditorSystemComponent();
        ~AudioEngineMAEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AudioControlsEditor::EditorImplPluginEventBus:
        void InitializeEditorImplPlugin() override;
        void ReleaseEditorImplPlugin() override;
        AudioControls::IAudioSystemEditor* GetEditorImplPlugin() override;

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        AZStd::unique_ptr<AudioControls::IAudioSystemEditor> m_editorImplPlugin;
    };
} // namespace AudioEngineMA
