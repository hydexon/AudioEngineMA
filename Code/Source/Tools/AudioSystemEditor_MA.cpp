#include "AudioSystemEditor_MA.h"

#include <AzCore/Utils/Utils.h>
#include <IAudioSystem.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/Console/IConsole.h>
#include <AudioFileUtils.h>

#include "AudioConnections.h"
#include "../Clients/Config_MA.h"
#include "../Clients/Common_MA.h"

using namespace AudioControls;

void EraseSubStr(AZStd::string& str, AZStd::string_view strToErase)
{
    auto pos = str.find(strToErase);
    if (pos != AZStd::string::npos)
    {
        str.erase(pos, strToErase.length());
    }
}


namespace AudioEngineMA
{

namespace MAControlTypes {
    enum Types {
        Invalid = 0,
        AudioFile = AUDIO_BIT(0),
        RTPC      = AUDIO_BIT(1)
    };
}

CAudioSystemEditor_MiniAudio::CAudioSystemEditor_MiniAudio()
{
    m_localizedParentControl.SetParent(&m_rootControl);
    m_rootControl.AddChild(&m_localizedParentControl);
}

void CAudioSystemEditor_MiniAudio::Reload()
{
    for(const auto& idControlPair : m_controls)
    {
        TControlPtr control = idControlPair.second;
        if(control)
        {
            control->SetPlaceholder(true);
        }
    }
    //TODO: Create global basic RTPCs.
    {
        m_currentLanguageName.clear();
        if(auto console = AZ::Interface<AZ::IConsole>::Get())
        {
            console->GetCvarValue("g_languageAudio", m_currentLanguageName);
            AZStd::to_lower(m_currentLanguageName.begin(), m_currentLanguageName.end());
        }
        AZStd::string parentName = "Localized Audio Files";
        if (!m_currentLanguageName.empty())
        {
            parentName += " (" + m_currentLanguageName + ")";
        }
        m_localizedParentControl.SetName(parentName);

        ScanAudioFilesAndCreateControlsRecursively(Config::AudioFilesPath);
    }
    m_connectionsByID.clear();

}

AudioControls::IAudioSystemControl *CAudioSystemEditor_MiniAudio::CreateControl(const AudioControls::SControlDef &controlDefinition)
{
    IAudioSystemControl* parent = controlDefinition.m_parentControl;

    AZStd::string fullName = controlDefinition.m_name;
    if(controlDefinition.m_isLocalized)
    {
        AZ::StringFunc::Path::Join(m_currentLanguageName.c_str(), fullName.c_str(), fullName);
    }

    CID id = GetID(fullName);

    IAudioSystemControl* control = GetControl(id);
    if(control)
    {
        if(control->IsPlaceholder())
        {
            control->SetPlaceholder(false);
            if(parent && parent->IsPlaceholder())
            {
                parent->SetPlaceholder(false);
            }
        }
        return control;
    }
    else
    {
        TControlPtr newControl = AZStd::make_shared<IAudioSystemControl>(controlDefinition.m_name, id, controlDefinition.m_type);
        if(!parent)
        {
            parent = &m_rootControl;
        }
        parent->AddChild(newControl.get());
        newControl->SetParent(parent);
        newControl->SetLocalized(controlDefinition.m_isLocalized);
        m_controls[id] = newControl;
        return newControl.get();
    }
}

AudioControls::IAudioSystemControl *CAudioSystemEditor_MiniAudio::GetRoot()
{
    return &m_rootControl;
}

AudioControls::IAudioSystemControl *CAudioSystemEditor_MiniAudio::GetControl(AudioControls::CID id) const
{
    auto it = m_controls.find(id);
    if(it != m_controls.end())
    {
        return it->second.get();
    }

    return nullptr;
}

AudioControls::EACEControlType CAudioSystemEditor_MiniAudio::ImplTypeToATLType(AudioControls::TImplControlType type) const
{
    switch(type)
    {
    case MAControlTypes::AudioFile:
        return eACET_PRELOAD;
    case MAControlTypes::RTPC:
        return eACET_RTPC;
    default:
        return eACET_NUM_TYPES;
    }
}

AudioControls::TImplControlTypeMask CAudioSystemEditor_MiniAudio::GetCompatibleTypes(AudioControls::EACEControlType atlControlType) const
{
    switch(atlControlType)
    {
    case eACET_PRELOAD:
    case eACET_TRIGGER:
        return MAControlTypes::AudioFile;
    case eACET_RTPC:
        return MAControlTypes::RTPC;
    default:
        return AUDIO_IMPL_INVALID_TYPE;
    }
}

AudioControls::TConnectionPtr CAudioSystemEditor_MiniAudio::CreateConnectionToControl([[maybe_unused]]AudioControls::EACEControlType atlControlType, AudioControls::IAudioSystemControl *middlewareControl)
{
    if(!middlewareControl)
    {
        return nullptr;
    }

    middlewareControl->SetConnected(true);
    ++m_connectionsByID[middlewareControl->GetId()];
    return AZStd::make_shared<IAudioConnection>(middlewareControl->GetId());
}

AudioControls::TConnectionPtr CAudioSystemEditor_MiniAudio::CreateConnectionFromXMLNode(AZ::rapidxml::xml_node<char> *node, AudioControls::EACEControlType atlControlType)
{
    using namespace Audio;
    if(!node)
    {
        return nullptr;
    }
    IAudioSystemControl* control = nullptr;
    TConnectionPtr connection;

    if(AZ::StringFunc::Equal(node->name(), MAXMLTags::AudioFileTag))
    {
        auto attr = node->first_attribute(MAXMLTags::AudioFilePathAttr);
        if(!attr || attr->value()[0] == '\0')
        {
            return nullptr;
        }

        const char* controlName = attr->value();
        bool isLocalized = false;

        attr = node->first_attribute(MAXMLTags::AudioFileLocalizedAttr);
        if(attr)
        {
            isLocalized = AZ::StringFunc::Equal(attr->value(), "true");
        }

        control = GetControlByName(controlName, isLocalized);
        if(!control)
        {
            control = CreateControl(SControlDef(controlName, MAControlTypes::AudioFile, isLocalized));
            control->SetPlaceholder(true);
        }

        //TODO switch(atlControlType)
        connection = AZStd::make_shared<IAudioConnection>(control->GetId());
    }

    return connection;
}

AZ::rapidxml::xml_node<char> *CAudioSystemEditor_MiniAudio::CreateXMLNodeFromConnection(const AudioControls::TConnectionPtr connection, const AudioControls::EACEControlType atlControlType)
{
    using namespace Audio;
    AZ_UNUSED(atlControlType);

    const IAudioSystemControl* control = GetControl(connection->GetID());
    if(!control)
    {
        return nullptr;
    }
    XmlAllocator& xmlAlloc(AudioControls::s_xmlAllocator);

    auto connNode = xmlAlloc.allocate_node(AZ::rapidxml::node_element);
    const auto controlType = static_cast<MAControlTypes::Types>(control->GetType());

    //TODO: Switch(controlType)
    if(controlType == MAControlTypes::AudioFile)
    {
        connNode->name(MAXMLTags::AudioFileTag);
        auto attr = xmlAlloc.allocate_attribute(MAXMLTags::AudioFilePathAttr, xmlAlloc.allocate_string(control->GetName().c_str()));
        connNode->append_attribute(attr);

        attr = xmlAlloc.allocate_attribute(MAXMLTags::AudioFileLocalizedAttr, xmlAlloc.allocate_string(control->IsLocalized() ? "true" : "false"));
        connNode->append_attribute(attr);
    }
    else
    {
        return nullptr;
    }

    return connNode;
}

void CAudioSystemEditor_MiniAudio::ConnectionRemoved(AudioControls::IAudioSystemControl *middlewareControl)
{
    int connectionCount = m_connectionsByID[middlewareControl->GetId()] -1;
    if(connectionCount <= 0)
    {
        connectionCount = 0;
        middlewareControl->SetConnected(false);
    }
    m_connectionsByID[middlewareControl->GetId()] = connectionCount;

}

const AZStd::string_view CAudioSystemEditor_MiniAudio::GetTypeIcon(AudioControls::TImplControlType type) const
{
    return AZStd::string_view();
}

const AZStd::string_view CAudioSystemEditor_MiniAudio::GetTypeIconSelected(AudioControls::TImplControlType type) const
{
    return AZStd::string_view();
}

AZStd::string CAudioSystemEditor_MiniAudio::GetName() const
{
    return "MiniAudio";
}

AZ::IO::FixedMaxPath CAudioSystemEditor_MiniAudio::GetDataPath() const
{
    auto projPath = AZ::IO::FixedMaxPath( AZ::Utils::GetProjectPath() );
    return (projPath / "Assets" / "Audio" / "MiniAudio" );
}

void CAudioSystemEditor_MiniAudio::DataSaved()
{

}

AudioControls::IAudioSystemControl *CAudioSystemEditor_MiniAudio::GetControlByName(AZStd::string name, bool isLocalized, AudioControls::IAudioSystemControl *parent) const
{
    if(isLocalized)
    {
        AZ::StringFunc::Path::Join(m_currentLanguageName.c_str(), name.c_str(), name);
    }

    return GetControl(GetID(name));
}

CID CAudioSystemEditor_MiniAudio::GetID(const AZStd::string_view name) const
{
    return Audio::AudioStringToID<CID>(name.data());
}

void CAudioSystemEditor_MiniAudio::ScanAudioFilesAndCreateControlsRecursively(AZ::IO::PathView scanDirPath, bool isInL10nDir)
{
    auto foundFiles = Audio::FindFilesInPath(scanDirPath.Native(), "*");
    for(const auto& foundFilePath : foundFiles)
    {
        AZ_Assert(AZ::IO::FileIOBase::GetInstance()->Exists(foundFilePath.c_str()),
                  "FindFiles found file '%s' but FileIO says it doesn't exist!", foundFilePath.c_str());
        if(AZ::IO::FileIOBase::GetInstance()->IsDirectory(foundFilePath.c_str()))
        {
            if(isInL10nDir)
            {
                ScanAudioFilesAndCreateControlsRecursively(foundFilePath, true);
            }
            else
            {
                auto nextDirPath = foundFilePath;
                bool isLocalizationDir = foundFilePath.Filename() == Config::LocalizationDirName;
                if(isLocalizationDir)
                {
                    if(!m_currentLanguageName.empty())
                    {
                        nextDirPath /= m_currentLanguageName;
                    }
                    else
                    {
                        continue;
                    }
                }
                ScanAudioFilesAndCreateControlsRecursively(nextDirPath, isLocalizationDir);
            }
        }
        else
        {
            AZStd::string fileExt;
            AZ::StringFunc::Path::GetExtension(foundFilePath.c_str(), fileExt, false);

            bool isFormatSupported = false;
            for(size_t i = 0; i < AZ_ARRAY_SIZE(Config::SupportedFileFormatsArray); ++i)
            {
                if(fileExt == Config::SupportedFileFormatsArray[i])
                {
                    isFormatSupported = true;
                    break;
                }
            }

            if(!isFormatSupported)
            {
                continue;
            }

            AZ::IO::FixedMaxPath filePathRelToAudioBaseDir(foundFilePath.c_str(), '/');
            {
                AZ::IO::FixedMaxPath pathPartToErase = Config::AudioFilesPath;
                if(isInL10nDir)
                {
                    pathPartToErase = pathPartToErase / Config::LocalizationDirName / m_currentLanguageName;
                }
                AZStd::string tmpstr = filePathRelToAudioBaseDir.Native().c_str();
                EraseSubStr(tmpstr, pathPartToErase.Native());
                filePathRelToAudioBaseDir = tmpstr;
            }
            filePathRelToAudioBaseDir.MakePreferred();
            filePathRelToAudioBaseDir = filePathRelToAudioBaseDir.RelativePath();
            IAudioSystemControl* parentControl = isInL10nDir ? &m_localizedParentControl : nullptr;

            SControlDef controlDef(
                filePathRelToAudioBaseDir.c_str(), MAControlTypes::AudioFile, isInL10nDir, parentControl);
            CreateControl(controlDef);
        }
    }
}


}