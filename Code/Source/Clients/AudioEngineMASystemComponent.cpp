
#include "AudioEngineMASystemComponent.h"

#include <AudioEngineMA/AudioEngineMATypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>
#include "AudioEngineImpl_MA.h"
namespace AudioEngineMA
{
    AZ_COMPONENT_IMPL(AudioEngineMASystemComponent, "AudioEngineMASystemComponent",
        AudioEngineMASystemComponentTypeId);

    void AudioEngineMASystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AudioEngineMASystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void AudioEngineMASystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("AudioEngineService"));
    }

    void AudioEngineMASystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("AudioEngineService"));
    }

    void AudioEngineMASystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("AudioSystemService"));
    }

    void AudioEngineMASystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        dependent.push_back(AZ_CRC_CE("AudioSystemService"));
    }

    AudioEngineMASystemComponent::AudioEngineMASystemComponent()
    {
    }

    AudioEngineMASystemComponent::~AudioEngineMASystemComponent()
    {
    }

    bool AudioEngineMASystemComponent::Initialize()
    {
        m_engineMA = AZStd::make_unique<AudioSystemImpl_MA>();
        if(m_engineMA)
        {
            Audio::SystemRequest::Initialize initReq;
            AZ::Interface<Audio::IAudioSystem>::Get()->PushRequestBlocking(AZStd::move(initReq));
        }
        return true;
    }

    void AudioEngineMASystemComponent::Release()
    {
        m_engineMA.reset();
    }

    void AudioEngineMASystemComponent::Init()
    {
    }

    void AudioEngineMASystemComponent::Activate()
    {
        Audio::Gem::EngineRequestBus::Handler::BusConnect();
    }

    void AudioEngineMASystemComponent::Deactivate()
    {
        Audio::Gem::EngineRequestBus::Handler::BusDisconnect();
    }


} // namespace AudioEngineMA
