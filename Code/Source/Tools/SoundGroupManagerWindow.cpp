#include "SoundGroupManagerWindow.h"
#include "ui_SoundGroupManagerWindow.h"

#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <AzQtComponents/Components/Widgets/CheckBox.h>

using namespace AudioEngineMA;

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

    SoundGroupManagerNotificationBus::Handler::BusConnect();
    //TODO: Request Sound Group Tree Layout.
}

SoundGroupManagerWindow::~SoundGroupManagerWindow()
{
    SoundGroupManagerNotificationBus::Handler::BusDisconnect();
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
        SoundGroupManagerRequestBus::QueueBroadcast(
                    &SoundGroupManagerRequestBus::Events::AddSoundGroup,
                    AZ::Name(newGroupName.toUtf8().constData()),
                    false, false,
                    selected ? AZ::Name(selected->text(0).toUtf8().constData()) : AZ::Name()
                    );
    }
    else
    {
        QMessageBox::warning(this, "Empty or invalid name", "Please input a valid sound group name (no duplicates)");
    }
}

void SoundGroupManagerWindow::onRemoveGroupClicked()
{

}

void SoundGroupManagerWindow::OnRequestCompleted_AddSoundGroup(bool success, AudioEngineMA::SoundGroupData data, AZ::Name parentGroup)
{
    if(!success)
    {
        //TODO: Show error dialog?
        return;
    }

    auto parentMatch = ui->tw_soundGroups->findItems(parentGroup.GetCStr(), Qt::MatchExactly | Qt::MatchRecursive, 0);
    QTreeWidgetItem* parent = nullptr;
    if(parentMatch.size() == 1)
    {
        parent = parentMatch[0];
    }

    QTreeWidgetItem* newItem = new QTreeWidgetItem();
    newItem->setText(0, data.m_groupName.GetCStr());
    newItem->setIcon(0, QIcon(":/AudioMAEditor/icons/ctrls/audio_file.png"));

    if(parent)
    {
        parent->addChild(newItem);
        parent->setExpanded(true);
    }
    else
    {
        ui->tw_soundGroups->addTopLevelItem(newItem);
    }
}

void SoundGroupManagerWindow::OnRequestCompleted_RemoveSoundGroup(bool success, AZ::Name soundGroupName)
{
    
}

void SoundGroupManagerWindow::OnRequestCompleted_RenameSoundGroup(bool success, AZ::Name newGroupName)
{
    
}
