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

    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chkbox_spatializationEnabled);

    connect(ui->btnAddGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onAddGroupClicked);
    connect(ui->btnRemoveGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onRemoveGroupClicked);
    connect(ui->btnRenameGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onRenameGroupClicked);
    connect(ui->btnClearSelection, &QPushButton::clicked, this, &SoundGroupManagerWindow::onClearSelectionClicked);
    connect(ui->tw_soundGroups, &QTreeWidget::currentItemChanged, this, &SoundGroupManagerWindow::onSoundGroupItemSelected);

    ui->btnRemoveGroup->setEnabled(false);
    ui->propertiesWidget->setEnabled(false);

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
    QTreeWidgetItem* selected = ui->tw_soundGroups->currentItem();
    if(!selected)
        return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove Sound Group", "Are you sure remove this sound group?");

    if(reply == QMessageBox::Yes)
    {
        SoundGroupManagerRequestBus::QueueBroadcast(
                    &SoundGroupManagerRequestBus::Events::RemoveSoundGroup,
                    AZ::Name(selected->text(0).toUtf8().constData())
                    );
    }

}

void SoundGroupManagerWindow::onRenameGroupClicked()
{
    QTreeWidgetItem* selected = ui->tw_soundGroups->currentItem();
    if(!selected)
        return;

    bool ok;
    QString newGrpName = QInputDialog::getText(
                this,
                "Rename Sound Group Name",
                "Type new sound group name",
                QLineEdit::Normal,
                selected->text(0),
                &ok);

    if(ok && !newGrpName.isEmpty())
    {
        if(newGrpName == selected->text(0))
            return;

        SoundGroupManagerRequestBus::QueueBroadcast(
                    &SoundGroupManagerRequestBus::Events::ChangeSoundGroupName,
                    AZ::Name(selected->text(0).toUtf8().constData()),
                    AZ::Name(newGrpName.toUtf8().constData())
                    );
    }

}

void SoundGroupManagerWindow::onClearSelectionClicked()
{
    ui->tw_soundGroups->clearSelection();
    ui->tw_soundGroups->setCurrentItem(nullptr);
    ui->propertiesWidget->setEnabled(false);
    ui->btnRemoveGroup->setEnabled(false);
    ui->btnClearSelection->setEnabled(false);
}

void SoundGroupManagerWindow::onSoundGroupItemSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    AZ_UNUSED(previous);
    if(!current)
        return;

    ui->btnRemoveGroup->setEnabled(true);
    ui->propertiesWidget->setEnabled(true);
    ui->btnClearSelection->setEnabled(true);
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
    if(!success)
        return;

    auto matches = ui->tw_soundGroups->findItems(soundGroupName.GetCStr(), Qt::MatchExactly | Qt::MatchRecursive, 0);
    if(matches.size() == 1)
    {
        delete matches[0];
    }
}

void SoundGroupManagerWindow::OnRequestCompleted_RenameSoundGroup(bool success, AZ::Name oldGroupName, AZ::Name newGroupName)
{
    if(!success)
        return;

    QTreeWidgetItem* item = nullptr;
    auto matches = ui->tw_soundGroups->findItems(oldGroupName.GetCStr(), Qt::MatchExactly | Qt::MatchRecursive, 0);
    if(matches.size() == 1)
    {
        item = matches[0];
    }

    if(!item)
        return;

    item->setText(0, newGroupName.GetCStr());
}
