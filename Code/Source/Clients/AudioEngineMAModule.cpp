
#include <AudioEngineMA/AudioEngineMATypeIds.h>
#include <AudioEngineMAModuleInterface.h>
#include "AudioEngineMASystemComponent.h"

namespace AudioEngineMA
{
    class AudioEngineMAModule
        : public AudioEngineMAModuleInterface
    {
    public:
        AZ_RTTI(AudioEngineMAModule, AudioEngineMAModuleTypeId, AudioEngineMAModuleInterface);
        AZ_CLASS_ALLOCATOR(AudioEngineMAModule, AZ::SystemAllocator);
    };
}// namespace AudioEngineMA

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), AudioEngineMA::AudioEngineMAModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_AudioEngineMA, AudioEngineMA::AudioEngineMAModule)
#endif
