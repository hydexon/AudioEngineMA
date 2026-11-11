#include "PropLayoutFilePathCtrl.h"
#include "Clients/Config_MA.h"
#include <QFileDialog>
#include <QHBoxLayout>

#include <AzCore/IO/FileIO.h>
#include <AzQtComponents/Components/Widgets/BrowseEdit.h>

namespace AudioEngineMA
{
    PropertyLayoutFilePathCtrl::PropertyLayoutFilePathCtrl(QWidget* parent, Qt::WindowFlags f)
        : QWidget(parent, f)
    {
        auto layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        m_browseEdit = new AzQtComponents::BrowseEdit(this);
        m_browseEdit->setClearButtonEnabled(true);
        layout->addWidget(m_browseEdit);

        connect(
            m_browseEdit, &AzQtComponents::BrowseEdit::attachedButtonTriggered, this,
            [this]()
            {
                QString filePath = QFileDialog::getOpenFileName(
                    this, "Open an audio bus layout", AZ::IO::FileIOBase::GetInstance()->GetAlias("@projectroot@"),
                    QString("Audio bus layout (*.%1)").arg(Config::MASoundGroupLayoutExt));

                QDir dir(AZ::IO::FileIOBase::GetInstance()->GetAlias("@projectroot@"));
                filePath = dir.relativeFilePath(filePath);

                if (!filePath.isEmpty())
                {
                    m_browseEdit->setText(filePath);
                    AZStd::string path = filePath.toUtf8().data();
                    emit ValueChanged(path);
                }
            });

        connect(
            m_browseEdit, &AzQtComponents::BrowseEdit::editingFinished, this,
            [this]()
            {
                AZStd::string path = m_browseEdit->text().toUtf8().data();
                emit ValueChanged(path);
            });
    }

    QWidget* PropertyLayoutFilePathCtrl::GetFirstInTabOrder()
    {
        return m_browseEdit;
    }

    QWidget* PropertyLayoutFilePathCtrl::GetLastInTabOrder()
    {
        return m_browseEdit;
    }

    void PropertyLayoutFilePathCtrl::SetValue(AZStd::string& value)
    {
        QString text = m_browseEdit->text();
        if (text.compare(value.data()) != 0)
        {
            m_browseEdit->blockSignals(true);
            m_browseEdit->setText(value.c_str());
            m_browseEdit->blockSignals(false);
        }
    }

    AZStd::string PropertyLayoutFilePathCtrl::GetValue() const
    {
        return m_browseEdit->text().toUtf8().data();
    }


    //---------------------------------------------------------------------------------------------------------------

    AZ::u32 PropertyLayoutFilePathHandler::GetHandlerName() const
    {
        return AZ_CRC("LayoutFilePath");
    }

    QWidget* PropertyLayoutFilePathHandler::GetFirstInTabOrder(PropertyLayoutFilePathCtrl* widget)
    {
        return widget->GetFirstInTabOrder();
    }

    QWidget* PropertyLayoutFilePathHandler::GetLastInTabOrder(PropertyLayoutFilePathCtrl* widget)
    {
        return widget->GetLastInTabOrder();
    }

    QWidget* PropertyLayoutFilePathHandler::CreateGUI(QWidget* pParent)
    {
        auto widget = aznew PropertyLayoutFilePathCtrl(pParent);
        connect(
            widget, &PropertyLayoutFilePathCtrl::ValueChanged, this,
            [widget]()
            {
                using namespace AzToolsFramework;
                PropertyEditorGUIMessages::Bus::Broadcast(&PropertyEditorGUIMessages::Bus::Events::RequestWrite, widget);
            });
        return widget;
    }

    void PropertyLayoutFilePathHandler::WriteGUIValuesIntoProperty(size_t index, PropertyLayoutFilePathCtrl* GUI,
        AZStd::basic_string<char>& instance, AzToolsFramework::InstanceDataNode* node)
    {
        const AZStd::string val = GUI->GetValue();
        instance = val.c_str();
    }

    bool PropertyLayoutFilePathHandler::ReadValuesIntoGUI(size_t index, PropertyLayoutFilePathCtrl* GUI,
        const AZStd::basic_string<char>& instance, AzToolsFramework::InstanceDataNode* node)
    {
        AZStd::string val = instance;
        GUI->SetValue(val);
        return false;
    }

    void RegisterLayoutFilePathHandler()
    {
        using namespace AzToolsFramework;
        PropertyTypeRegistrationMessages::Bus::Broadcast(
            &PropertyTypeRegistrationMessages::Bus::Events::RegisterPropertyType, aznew PropertyLayoutFilePathHandler);
    }
}
