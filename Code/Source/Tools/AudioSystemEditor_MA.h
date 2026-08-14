#pragma once

#include <IAudioSystemEditor.h>
#include <IAudioConnection.h>
#include <IAudioSystemControl.h>

namespace AudioEngineMA
{
    class CAudioSystemEditor_MiniAudio :
            public AudioControls::IAudioSystemEditor
    {
    public:
        CAudioSystemEditor_MiniAudio();
        ~CAudioSystemEditor_MiniAudio() = default;

        void Reload() override;
        AudioControls::IAudioSystemControl *CreateControl(const AudioControls::SControlDef &controlDefinition) override;
        AudioControls::IAudioSystemControl *GetRoot() override;
        AudioControls::IAudioSystemControl *GetControl(AudioControls::CID id) const override;
        AudioControls::EACEControlType ImplTypeToATLType(AudioControls::TImplControlType type) const override;
        AudioControls::TImplControlTypeMask GetCompatibleTypes(AudioControls::EACEControlType atlControlType) const override;
        AudioControls::TConnectionPtr CreateConnectionToControl(AudioControls::EACEControlType atlControlType, AudioControls::IAudioSystemControl *middlewareControl) override;
        AudioControls::TConnectionPtr CreateConnectionFromXMLNode(AZ::rapidxml::xml_node<char> *node, AudioControls::EACEControlType atlControlType) override;
        AZ::rapidxml::xml_node<char> *CreateXMLNodeFromConnection(const AudioControls::TConnectionPtr connection, const AudioControls::EACEControlType atlControlType) override;
        void ConnectionRemoved(AudioControls::IAudioSystemControl* middlewareControl) override;
        const AZStd::string_view GetTypeIcon(AudioControls::TImplControlType type) const override;
        const AZStd::string_view GetTypeIconSelected(AudioControls::TImplControlType type) const override;
        AZStd::string GetName() const override;
        AZ::IO::FixedMaxPath GetDataPath() const override;
        void DataSaved() override;

    private:
        AZStd::string m_currentLanguageName;
        AudioControls::IAudioSystemControl m_rootControl;
        AudioControls::IAudioSystemControl m_localizedParentControl;

        using TControlPtr = AZStd::shared_ptr<AudioControls::IAudioSystemControl>;
        using TControlMap = AZStd::unordered_map<AudioControls::CID, TControlPtr>;
        TControlMap m_controls;

        using TConnectionsMap = AZStd::unordered_map<AudioControls::CID, int>;
        TConnectionsMap m_connectionsByID;

        AudioControls::IAudioSystemControl* GetControlByName(AZStd::string name, bool isLocalized, AudioControls::IAudioSystemControl* parent = nullptr) const;
        AudioControls::CID GetID(const AZStd::string_view name) const;

        void ScanAudioFilesAndCreateControlsRecursively(AZ::IO::PathView scanDirPath, bool isInL10nDir = false);
    };
}