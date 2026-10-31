#include "AudioFileToRTPCParamsWidget.h"
#include "ui_AudioFileToRTPCParamsWidget.h"

#include <float.h>
#include "AudioConnections.h"
#include "../Clients/ATLData.h"

using namespace AudioEngineMA;

AudioFileToRTPCParamsWidget::AudioFileToRTPCParamsWidget(AudioControls::TConnectionPtr connection, QWidget *parent) :
    QWidget(parent),
    m_connection(connection),
    ui(new Ui::AudioFileToRTPCParamsWidget)
{
    ui->setupUi(this);

    ui->dsb_mult->setMinimum(FLT_MIN);
    ui->dsb_mult->setMaximum(FLT_MAX);

    ui->dsb_shift->setMinimum(FLT_MIN);
    ui->dsb_shift->setMaximum(FLT_MAX);

    connect(ui->cb_rtpcType, &QComboBox::currentIndexChanged, this, &AudioFileToRTPCParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->cb_scope, &QComboBox::currentIndexChanged, this, &AudioFileToRTPCParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_mult, &QDoubleSpinBox::valueChanged, this, &AudioFileToRTPCParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_shift, &QDoubleSpinBox::valueChanged, this, &AudioFileToRTPCParamsWidget::UpdateConnectionFromWidgets);

    UpdateWidgetsFromConnection();
}

AudioFileToRTPCParamsWidget::~AudioFileToRTPCParamsWidget()
{
    delete ui;
}

void AudioFileToRTPCParamsWidget::UpdateConnectionFromWidgets()
{
    if(m_inUpdateWidgetsFromConnection)
        return;

    AudioFileToRTPCConnection* conn = static_cast<AudioFileToRTPCConnection*>(m_connection.get());
    conn->m_params.m_type = static_cast<AudioFileRTPC::Type>(ui->cb_rtpcType->currentIndex());
    conn->m_params.m_perObject = ui->cb_scope->currentIndex() == 1;
    conn->m_params.m_mult  = ui->dsb_mult->value();
    conn->m_params.m_shift = ui->dsb_shift->value();

    emit PropertiesChanged();
}

void AudioFileToRTPCParamsWidget::UpdateWidgetsFromConnection()
{
    m_inUpdateWidgetsFromConnection = true;

    AudioFileToRTPCConnection* conn = static_cast<AudioFileToRTPCConnection*>(m_connection.get());
    ui->cb_rtpcType->setCurrentIndex(conn->m_params.m_type);
    conn->m_params.m_perObject ? ui->cb_scope->setCurrentIndex(1) : ui->cb_scope->setCurrentIndex(0);
    ui->dsb_mult->setValue(conn->m_params.m_mult);
    ui->dsb_shift->setValue(conn->m_params.m_shift);

    m_inUpdateWidgetsFromConnection = false;

}
