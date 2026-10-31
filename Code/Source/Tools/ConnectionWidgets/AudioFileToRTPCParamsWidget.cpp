#include "AudioFileToRTPCParamsWidget.h"
#include "ui_AudioFileToRTPCParamsWidget.h"

#include <float.h>

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

    emit PropertiesChanged();
}

void AudioFileToRTPCParamsWidget::UpdateWidgetsFromConnection()
{
    m_inUpdateWidgetsFromConnection = true;

    m_inUpdateWidgetsFromConnection = false;

}
