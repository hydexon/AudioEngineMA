#include "AudioFileToTriggerParamsWidget.h"
#include "ui_AudioFileToTriggerParamsWidget.h"

#include <AudioConnections.h>
#include <AzQtComponents/Components/Widgets/CheckBox.h>

using namespace AudioEngineMA;

AudioFileToTriggerParamsWidget::AudioFileToTriggerParamsWidget(AudioControls::TConnectionPtr connection, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioFileToTriggerParamsWidget),
    m_connection(connection)
{
    ui->setupUi(this);

    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chkBox_loop);
    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chkBox_spatialization);

    connect(ui->cb_SoundAction, &QComboBox::currentIndexChanged, this, &AudioFileToTriggerParamsWidget::OnSoundActionChanged);
    connect(ui->cb_SoundAction, &QComboBox::currentIndexChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    //TODO: SoundGroup.
    connect(ui->dsb_Volume, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->chkBox_loop, &QCheckBox::checkStateChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->chkBox_spatialization, &QCheckBox::checkStateChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->cb_attenuationMode, &QComboBox::currentIndexChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->cb_panMode, &QComboBox::currentIndexChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_rolloff, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_minDistance, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_maxDistance, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_innerAngle, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_outerAngle, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_outerGrain, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_dopplerFactor, &QDoubleSpinBox::valueChanged, this, &AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets);

    UpdateWidgetsFromConnection();
}

AudioFileToTriggerParamsWidget::~AudioFileToTriggerParamsWidget()
{
    delete ui;
}

void AudioFileToTriggerParamsWidget::OnSoundActionChanged(int index)
{
    ui->parentFrame->setEnabled(index == SoundAction::Start);
}

void AudioFileToTriggerParamsWidget::UpdateConnectionFromWidgets()
{

    if(m_inUpdateWidgetsFromConnection)
        return;

    AudioFileToTriggerConnection* conn = static_cast<AudioFileToTriggerConnection*>(m_connection.get());
    conn->m_params.m_soundAction = static_cast<SoundAction::Type>(ui->cb_SoundAction->currentIndex());
    //TODO: Sound Groups.
    conn->m_params.m_volume  = ui->dsb_Volume->value();
    conn->m_params.m_looping = ui->chkBox_loop->isChecked();
    conn->m_params.m_spatialization = ui->chkBox_spatialization->isChecked();
    conn->m_params.m_attenuationMode = static_cast<AttenuationMode::Type>(ui->cb_attenuationMode->currentIndex());
    conn->m_params.m_panMode = static_cast<PanMode::Type>(ui->cb_panMode->currentIndex());
    conn->m_params.m_rolloff = ui->dsb_rolloff->value();
    conn->m_params.m_minDistance = ui->dsb_minDistance->value();
    conn->m_params.m_maxDistance = ui->dsb_maxDistance->value();
    conn->m_params.m_coneinnerAngle = ui->dsb_innerAngle->value();
    conn->m_params.m_coneOuterAngle = ui->dsb_outerAngle->value();
    conn->m_params.m_coneOuterGrain = ui->dsb_outerGrain->value();
    conn->m_params.m_dopplerFactor  = ui->dsb_dopplerFactor->value();

    emit PropertiesChanged();
}

void AudioFileToTriggerParamsWidget::UpdateWidgetsFromConnection()
{
    m_inUpdateWidgetsFromConnection = true;

    AudioFileToTriggerConnection* conn = static_cast<AudioFileToTriggerConnection*>(m_connection.get());

    ui->cb_SoundAction->setCurrentIndex(conn->m_params.m_soundAction);
    //TODO: SoundGroups.
    ui->dsb_Volume->setValue(conn->m_params.m_volume);
    ui->chkBox_loop->setChecked(conn->m_params.m_looping);
    ui->chkBox_spatialization->setChecked(conn->m_params.m_spatialization);
    ui->cb_attenuationMode->setCurrentIndex(conn->m_params.m_attenuationMode);
    ui->cb_panMode->setCurrentIndex(conn->m_params.m_panMode);
    ui->dsb_rolloff->setValue(conn->m_params.m_rolloff);
    ui->dsb_minDistance->setValue(conn->m_params.m_minDistance);
    ui->dsb_maxDistance->setValue(conn->m_params.m_maxDistance);
    ui->dsb_innerAngle->setValue(conn->m_params.m_coneinnerAngle);
    ui->dsb_outerAngle->setValue(conn->m_params.m_coneOuterAngle);
    ui->dsb_outerGrain->setValue(conn->m_params.m_coneOuterGrain);
    ui->dsb_dopplerFactor->setValue(conn->m_params.m_dopplerFactor);

    m_inUpdateWidgetsFromConnection = false;
}
