#include "ATLEntities_MA.h"

#include <AzCore/StringFunc/StringFunc.h>

namespace AudioEngineMA
{

bool SATLRtpcImplData_MA::ReadFromXML(const AZ::rapidxml::xml_node<char> &node)
{
    if(AZ::StringFunc::Equal(node.name(), MAXMLTags::AudioFileTag))
    {
        m_type = RtpcImplementation::AudioFile;
        auto attr = node.first_attribute(MAXMLTags::AudioFilePathAttr);
        if(!attr)
            return false;

        m_audioFile.m_audioFilePath = attr->value();
        if(m_audioFile.m_audioFilePath.empty())
            return false;

        return m_audioFile.m_params.ReadFromXML(node);
    }
    else
    {
        return m_global.m_params.ReadFromXML(node);
    }
}

	
}