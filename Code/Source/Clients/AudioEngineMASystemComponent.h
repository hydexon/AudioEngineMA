
#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AudioEngineMA/AudioEngineMABus.h>

#include <IAudioSystem.h>
#include <IAudioSystemImplementation.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

namespace AudioEngineMA
{
    class AudioEngineMASystemComponent
        : public AZ::Component
        , protected Audio::Gem::EngineRequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(AudioEngineMASystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        AudioEngineMASystemComponent();
        ~AudioEngineMASystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // Audio::Gem::EngineRequestBus interface implementation
        bool Initialize() override;
        void Release() override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

    private:
        AZStd::unique_ptr<Audio::AudioSystemImplementation> m_engineMA;
    };

} // namespace AudioEngineMA
