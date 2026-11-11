
#pragma once

#include "../CodeComponent.h"

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace ${GemName}
{
    /*
    * TODO: Register this component in your Gem's Editor AZ::Module interface by inserting the following:
    *       EditorCodeComponent::CreateDescriptor(),
    *
    * Editor components wrap the corresponding runtime component. In the editor, this component
    * is placed on entities. At export/game-mode, BuildGameEntity() instantiates the runtime component.
    */

    class EditorCodeComponent
        : public AzToolsFramework::Components::EditorComponentBase
    {
    public:
        AZ_EDITOR_COMPONENT(EditorCodeComponent, "{5F4D9050-46EE-48A2-A527-E076057324F5}", AzToolsFramework::Components::EditorComponentBase);

        /*
        * Reflects component data into the reflection contexts.
        * The edit context reflection here controls how the component appears in the Editor Inspector.
        */
        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        /*
        * Called at game export and when entering game mode.
        * Add the runtime component to the game entity, transferring any serialized data as needed.
        */
        void BuildGameEntity(AZ::Entity* gameEntity) override;
    };

} // namespace ${GemName}
