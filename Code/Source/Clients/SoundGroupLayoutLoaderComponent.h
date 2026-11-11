#pragma once

#include <AzCore/Component/Component.h>

namespace AudioEngineMA
{
    class SoundGroupLayoutLoaderComponent : public AZ::Component
    {
    public:
        AZ_COMPONENT(SoundGroupLayoutLoaderComponent, "{67B620E4-35EE-4155-B4C0-0FDBBAC5E77B}", AZ::Component);
        static void Reflect(AZ::ReflectContext* context);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
        // ~AZ::Component
    private:
        AZStd::string m_layoutFilePath;

    };
    
}