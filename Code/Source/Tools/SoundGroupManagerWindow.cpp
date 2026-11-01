#include "SoundGroupManagerWindow.h"
#include "ui_SoundGroupManagerWindow.h"

#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QLineEdit>
#include <AzQtComponents/Components/Widgets/CheckBox.h>

SoundGroupManagerWindow::SoundGroupManagerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SoundGroupManagerWindow)
{
    ui->setupUi(this);

    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chkbox_DisablePtich);
    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chkbox_DisableSpatial);

    connect(ui->btnAddGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onAddGroupClicked);
    connect(ui->btnRemoveGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onRemoveGroupClicked);

    ui->btnRemoveGroup->setEnabled(false);
}

SoundGroupManagerWindow::~SoundGroupManagerWindow()
{
    delete ui;
}

void SoundGroupManagerWindow::onAddGroupClicked()
{
    bool ok;
    QString newGroupName = QInputDialog::getText(
                this,
                "New Sound Group Name",
                "Type new sound group name:",
                QLineEdit::Normal,
                "",
                &ok);

    if(ok && !newGroupName.isEmpty())
    {
        QTreeWidgetItem* selected = ui->tw_soundGroups->currentItem();
        QTreeWidgetItem* newItem = new QTreeWidgetItem();
        newItem->setText(0, newGroupName);
        newItem->setIcon(0, QIcon(":/AudioMAEditor/icons/ctrls/audio_file.png"));

        if(selected)
        {
            selected->addChild(newItem);
            selected->setExpanded(true);
        }
        else
        {
            ui->tw_soundGroups->addTopLevelItem(newItem);
        }
    }
}

void SoundGroupManagerWindow::onRemoveGroupClicked()
{

}
