
#pragma once

#include "../SoundGroupLayoutLoaderComponentComponent.h"

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace ${GemName}
{
    /*
    * TODO: Register this component in your Gem's Editor AZ::Module interface by inserting the following:
    *       EditorSoundGroupLayoutLoaderComponentComponent::CreateDescriptor(),
    *
    * Editor components wrap the corresponding runtime component. In the editor, this component
    * is placed on entities. At export/game-mode, BuildGameEntity() instantiates the runtime component.
    */

    class EditorSoundGroupLayoutLoaderComponentComponent
        : public AzToolsFramework::Components::EditorComponentBase
    {
    public:
        AZ_EDITOR_COMPONENT(EditorSoundGroupLayoutLoaderComponentComponent, "{963BD002-7BCA-4CBC-8DF7-77869134AA9C}", AzToolsFramework::Components::EditorComponentBase);

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
