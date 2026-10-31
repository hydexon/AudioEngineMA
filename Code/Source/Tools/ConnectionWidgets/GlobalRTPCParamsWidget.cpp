#include "GlobalRTPCParamsWidget.h"
#include "ui_GlobalRTPCParamsWidget.h"

#include <float.h>
#include "AudioConnections.h"
#include "../Clients/ATLData.h"

using namespace AudioEngineMA;

GlobalRTPCParamsWidget::GlobalRTPCParamsWidget(AudioControls::TConnectionPtr connection, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GlobalRTPCParamsWidget),
    m_connection(connection)
{
    ui->setupUi(this);

    ui->dsb_mult->setMinimum(FLT_MIN);
    ui->dsb_mult->setMaximum(FLT_MAX);

    ui->dsb_shift->setMinimum(FLT_MIN);
    ui->dsb_shift->setMaximum(FLT_MAX);

    connect(ui->dsb_mult, &QDoubleSpinBox::valueChanged, this, &GlobalRTPCParamsWidget::UpdateConnectionFromWidgets);
    connect(ui->dsb_shift, &QDoubleSpinBox::valueChanged, this, &GlobalRTPCParamsWidget::UpdateConnectionFromWidgets);

    UpdateWidgetsFromConnection();
}

GlobalRTPCParamsWidget::~GlobalRTPCParamsWidget()
{
    delete ui;
}

void GlobalRTPCParamsWidget::UpdateConnectionFromWidgets()
{

    if(m_inUpdateWidgetsFromConnection)
        return;

    GlobalRTPCConnection* conn = static_cast<GlobalRTPCConnection*>(m_connection.get());
    conn->m_params.m_type  = GlobalRTPC::GlobalVolume; //Just in case, remove when we have more global rtpcs, besides setting global volume.
    conn->m_params.m_mult  = ui->dsb_mult->value();
    conn->m_params.m_shift = ui->dsb_shift->value();

    emit PropertiesChanged();
}

void GlobalRTPCParamsWidget::UpdateWidgetsFromConnection()
{
    m_inUpdateWidgetsFromConnection = true;

    GlobalRTPCConnection* conn = static_cast<GlobalRTPCConnection*>(m_connection.get());
    ui->dsb_mult->setValue(conn->m_params.m_mult);
    ui->dsb_shift->setValue(conn->m_params.m_shift);

    m_inUpdateWidgetsFromConnection = false;
}
