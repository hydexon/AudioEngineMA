#include "SoundGroupManagerWindow.h"
#include "ui_SoundGroupManagerWindow.h"

#include "Clients/Config_MA.h"

#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QFileInfo>

#include <AzQtComponents/Components/Widgets/CheckBox.h>
#include <AzCore/IO/FileIO.h>

using namespace AudioEngineMA;
#define GET_GROUP() \
    QTreeWidgetItem* curr = ui->tw_soundGroups->currentItem(); \
    if(!curr) \
        return; \
    auto group = AZ::Name(curr->text(0).toUtf8().constData()); \


SoundGroupManagerWindow::SoundGroupManagerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SoundGroupManagerWindow)
{
    ui->setupUi(this);

    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chkbox_spatializationEnabled);

    connect(ui->actionNew_Layout, &QAction::triggered, this, &SoundGroupManagerWindow::onNewLayoutActionTriggered);
    connect(ui->actionOpen_Layout, &QAction::triggered, this, &SoundGroupManagerWindow::onLoadLayoutActionTriggered);
    connect(ui->actionSave_Layout, &QAction::triggered, this, &SoundGroupManagerWindow::onSaveLayoutActionTriggered);
    connect(ui->actionSave_Layout_As, &QAction::triggered, this, &SoundGroupManagerWindow::onSaveLayoutAsActionTriggered);

    connect(ui->btnAddGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onAddGroupClicked);
    connect(ui->btnRemoveGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onRemoveGroupClicked);
    connect(ui->btnRenameGroup, &QPushButton::clicked, this, &SoundGroupManagerWindow::onRenameGroupClicked);
    connect(ui->btnClearSelection, &QPushButton::clicked, this, &SoundGroupManagerWindow::onClearSelectionClicked);
    connect(ui->tw_soundGroups, &QTreeWidget::currentItemChanged, this, &SoundGroupManagerWindow::onSoundGroupItemSelected);

    ConnectPropertyWidgetSignals();

    ui->btnRemoveGroup->setEnabled(false);
    ui->propertiesWidget->setEnabled(false);
    ui->lblFile->setVisible(false);

    SoundGroupManagerNotificationBus::Handler::BusConnect();
    SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::RequestSoundGroupLayout);
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
    ResetSelectionState();
}

void SoundGroupManagerWindow::onSoundGroupItemSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    AZ_UNUSED(previous);
    if(!current)
        return;

    if(current == previous)
        return;

    auto group = AZ::Name(current->text(0).toUtf8().constData());
    SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::RequestSoundGroupData, group);

    ui->btnRemoveGroup->setEnabled(true);
    ui->btnClearSelection->setEnabled(true);
}

void SoundGroupManagerWindow::onNewLayoutActionTriggered()
{
    SetCurrentSavePath(QString());
    ResetSelectionState();
    SoundGroupManagerRequestBus::QueueBroadcast(
                &SoundGroupManagerRequestBus::Events::RequestSoundGroupLayout);
}

void SoundGroupManagerWindow::onLoadLayoutActionTriggered()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Open an Sound Group Layout", AZ::IO::FileIOBase::GetInstance()->GetAlias("@projectroot@"),
                QString("MiniAudio Sound Group Layout (*.%1)").arg(Config::MASoundGroupLayoutExt));

    if(path.isEmpty())
    {
        return;
    }

    SetCurrentSavePath(path);
    m_loadLayoutWasRequested = true;

    SoundGroupManagerRequestBus::QueueBroadcast(
                &SoundGroupManagerRequestBus::Events::LoadSoundGroupLayout,
                path.toUtf8().data());
}

void SoundGroupManagerWindow::onSaveLayoutActionTriggered()
{
    SaveLayoutFile(false);
}

void SoundGroupManagerWindow::onSaveLayoutAsActionTriggered()
{
    SaveLayoutFile(true);
}

void SoundGroupManagerWindow::ConnectPropertyWidgetSignals()
{
    connect(ui->chkbox_spatializationEnabled, &QCheckBox::checkStateChanged, this, [this](bool check)  {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupSpatializationEnabled, group , check);
    });

    connect(ui->cb_attenuationMode, &QComboBox::currentIndexChanged, this, [this](int index) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupAttenuationMode, group, static_cast<AttenuationMode::Type>(index));
    });

    connect(ui->cb_positioning, &QComboBox::currentIndexChanged, this, [this](int index) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPositioning, group, static_cast<PositioningMode::Type>(index));
    });

    connect(ui->cb_panMode, &QComboBox::currentIndexChanged, this, [this](int index) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPanMode, group, static_cast<PanMode::Type>(index));
    });

    connect(ui->dsb_volume, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupVolume, group, value);
    });

    connect(ui->dsb_pitch, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPitch, group, value);
    });

    connect(ui->dsb_pan, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupPan, group, value);
    });

    connect(ui->dsb_rolloffFactor, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupRolloff, group, value);
    });

    connect(ui->dsb_dopplerFactor, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupDopplerFactor, group, value);
    });

    connect(ui->dsb_distanceMin, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMinDistance, group, value);
    });

    connect(ui->dsb_distanceMax, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMaxDistance, group, value);
    });

    connect(ui->dsb_gainsMin, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMinGains, group, value);
    });

    connect(ui->dsb_gainsMax, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupMaxGains, group, value);
    });

    connect(ui->dsb_innerAngle, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupAttenuationCone, group,
                                                    value, ui->dsb_outerAngle->value(), ui->dsb_outerGrain->value());
    });

    connect(ui->dsb_outerAngle, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupAttenuationCone, group,
                                                    ui->dsb_innerAngle->value(), value, ui->dsb_outerGrain->value());
    });

    connect(ui->dsb_outerGrain, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        GET_GROUP();
        SoundGroupManagerRequestBus::QueueBroadcast(&SoundGroupManagerRequestBus::Events::SetSoundGroupAttenuationCone, group,
                                                    ui->dsb_innerAngle->value(), ui->dsb_outerAngle->value(), value);
    });
}

void SoundGroupManagerWindow::BlockPropertySignals(bool value)
{
    ui->chkbox_spatializationEnabled->blockSignals(value);
    ui->cb_attenuationMode->blockSignals(value);
    ui->cb_positioning->blockSignals(value);
    ui->cb_panMode->blockSignals(value);

    ui->dsb_volume->blockSignals(value);
    ui->dsb_pitch->blockSignals(value);
    ui->dsb_pan->blockSignals(value);
    ui->dsb_rolloffFactor->blockSignals(value);
    ui->dsb_dopplerFactor->blockSignals(value);
    ui->dsb_distanceMin->blockSignals(value);
    ui->dsb_distanceMax->blockSignals(value);
    ui->dsb_innerAngle->blockSignals(value);
    ui->dsb_outerAngle->blockSignals(value);
    ui->dsb_outerGrain->blockSignals(value);
    ui->dsb_gainsMin->blockSignals(value);
    ui->dsb_gainsMax->blockSignals(value);
}

void SoundGroupManagerWindow::SetCurrentSavePath(const QString &path)
{
    m_currentLayoutPath = path;

    if(!path.isEmpty())
    {
        ui->lblFile->setText(QString("<b>Sound Group Layout:</b> %1").arg(QFileInfo(m_currentLayoutPath).baseName()));
        ui->lblFile->setVisible(true);
    }
    else
    {
        ui->lblFile->setVisible(false);
    }
}

void SoundGroupManagerWindow::SaveLayoutFile(bool saveAs)
{
    if(saveAs || m_currentLayoutPath.isEmpty())
    {
        QString path = QFileDialog::getSaveFileName(
                    this, "Save the Sound Group Layout",
                    AZ::IO::FileIOBase::GetInstance()->GetAlias("@projectroot@"),
                    QString("MiniAudio Sound Group Layout (*.%1)").arg(Config::MASoundGroupLayoutExt));


        if(!path.isEmpty())
            SetCurrentSavePath(path);
    }

    if(!m_currentLayoutPath.isEmpty())
    {
        SoundGroupManagerRequestBus::QueueBroadcast(
                    &SoundGroupManagerRequestBus::Events::SaveSoundGroupLayout,
                    m_currentLayoutPath.toUtf8().data());
    }

}

void SoundGroupManagerWindow::ResetSelectionState()
{
    ui->tw_soundGroups->clearSelection();
    ui->tw_soundGroups->setCurrentItem(nullptr);
    ui->propertiesWidget->setEnabled(false);
    ui->btnRemoveGroup->setEnabled(false);
    ui->btnClearSelection->setEnabled(false);
}

void SoundGroupManagerWindow::PopulateChildItems(QTreeWidgetItem *parent, AZStd::vector<AudioEngineMA::SoundGroupData> &data)
{
    if(!parent)
        return;

    for(auto child : data)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, child.m_groupName.GetCStr());
        item->setIcon(0, QIcon(":/AudioMAEditor/icons/ctrls/audio_file.png"));
        PopulateChildItems(item, child.m_children);
        parent->addChild(item);
        item->setExpanded(true);
    }
}

void SoundGroupManagerWindow::OnRequestCompleted_AddSoundGroup(
        bool success, AudioEngineMA::SoundGroupData data, AZ::Name parentGroup) {
    if (!success) {
        // TODO: Show error dialog?
        return;
    }

    auto parentMatch = ui->tw_soundGroups->findItems(
                parentGroup.GetCStr(), Qt::MatchExactly | Qt::MatchRecursive, 0);
    QTreeWidgetItem *parent = nullptr;
    if (parentMatch.size() == 1) {
        parent = parentMatch[0];
    }

    QTreeWidgetItem *newItem = new QTreeWidgetItem();
    newItem->setText(0, data.m_groupName.GetCStr());
    newItem->setIcon(0, QIcon(":/AudioMAEditor/icons/ctrls/audio_file.png"));

    if (parent) {
        parent->addChild(newItem);
        parent->setExpanded(true);
    } else {
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

void SoundGroupManagerWindow::OnUpdateSoundGroupLayout(AudioEngineMA::SoundGroupLayoutData sndGroupLayout)
{
    if(m_loadLayoutWasRequested)
    {
        m_loadLayoutWasRequested = false;
    }
    else
    {
        SetCurrentSavePath(QString());
    }

    ResetSelectionState();
    ui->tw_soundGroups->clear();

    for(auto group : sndGroupLayout.m_groups)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, group.m_groupName.GetCStr());
        item->setIcon(0, QIcon(":/AudioMAEditor/icons/ctrls/audio_file.png"));

        PopulateChildItems(item, group.m_children);
        ui->tw_soundGroups->addTopLevelItem(item);
        item->setExpanded(true);
    }
}

void SoundGroupManagerWindow::OnUpdateSoundGroup(bool success, AudioEngineMA::SoundGroupData groupData)
{
    if(!success)
        return;

    blockSignals(true);

    ui->chkbox_spatializationEnabled->setChecked(groupData.m_spatialization);

    ui->cb_attenuationMode->setCurrentIndex(groupData.m_attenuationMode);
    ui->cb_positioning->setCurrentIndex(groupData.m_positioningType);
    ui->cb_panMode->setCurrentIndex(groupData.m_panMode);

    ui->dsb_volume->setValue(groupData.m_volume);
    ui->dsb_pitch->setValue(groupData.m_pitch);
    ui->dsb_pan->setValue(groupData.m_pan);

    ui->dsb_dopplerFactor->setValue(groupData.m_dopplerFactor);
    ui->dsb_rolloffFactor->setValue(groupData.m_rolloff);
    ui->dsb_distanceMin->setValue(groupData.m_minDistance);
    ui->dsb_distanceMax->setValue(groupData.m_maxDistance);
    ui->dsb_gainsMin->setValue(groupData.m_minGains);
    ui->dsb_gainsMax->setValue(groupData.m_maxGains);
    ui->dsb_innerAngle->setValue(groupData.m_coneinnerAngle);
    ui->dsb_outerAngle->setValue(groupData.m_coneOuterAngle);
    ui->dsb_outerGrain->setValue(groupData.m_coneOuterGrain);

    blockSignals(false);
    ui->propertiesWidget->setEnabled(true);
}

#undef GET_GROUP