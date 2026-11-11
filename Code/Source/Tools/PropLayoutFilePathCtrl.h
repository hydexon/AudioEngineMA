#pragma once

#if !defined(Q_MOC_RUN)
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/std/string/string.h>
#include <AzToolsFramework/UI/PropertyEditor/PropertyEditorAPI.h>

#include <QWidget>
#endif

class QLineEdit;
class QToolButton;

namespace AzQtComponents
{
    class BrowseEdit;
}

namespace AudioEngineMA
{
    class PropertyLayoutFilePathCtrl : public QWidget
    {
        Q_OBJECT;
    public:
        AZ_CLASS_ALLOCATOR(PropertyLayoutFilePathCtrl, AZ::SystemAllocator, 0);
        explicit PropertyLayoutFilePathCtrl(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
        ~PropertyLayoutFilePathCtrl() = default;

        QWidget* GetFirstInTabOrder();
        QWidget* GetLastInTabOrder();

        void SetValue(AZStd::string& value);
        AZStd::string GetValue() const;

    signals:
        void ValueChanged(AZStd::string& newValue);
    private:
        AzQtComponents::BrowseEdit* m_browseEdit = nullptr;
    };

    class PropertyLayoutFilePathHandler
        : QObject
        , public AzToolsFramework::PropertyHandler<AZStd::string, PropertyLayoutFilePathCtrl>
    {
        Q_OBJECT

    public:
        AZ_CLASS_ALLOCATOR(PropertyLayoutFilePathHandler, AZ::SystemAllocator, 0);

        AZ::u32 GetHandlerName() const override;
        QWidget* GetFirstInTabOrder(PropertyLayoutFilePathCtrl* widget) override;
        QWidget* GetLastInTabOrder(PropertyLayoutFilePathCtrl* widget) override;

        QWidget* CreateGUI(QWidget* pParent) override;
        void WriteGUIValuesIntoProperty(size_t index, PropertyLayoutFilePathCtrl* GUI, AZStd::basic_string<char>& instance, AzToolsFramework::InstanceDataNode* node) override;
        bool ReadValuesIntoGUI(size_t index, PropertyLayoutFilePathCtrl* GUI, const AZStd::basic_string<char>& instance, AzToolsFramework::InstanceDataNode* node) override;
    };

    void RegisterLayoutFilePathHandler();
}
