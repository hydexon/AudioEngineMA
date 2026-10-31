#ifndef GLOBALRTPCPARAMSWIDGET_H
#define GLOBALRTPCPARAMSWIDGET_H

#include <QWidget>
#include <ACETypes.h>

namespace Ui {
class GlobalRTPCParamsWidget;
}

class GlobalRTPCParamsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GlobalRTPCParamsWidget(AudioControls::TConnectionPtr connection, QWidget *parent = nullptr);
    ~GlobalRTPCParamsWidget();
signals:
    void PropertiesChanged();

private slots:
    void UpdateConnectionFromWidgets();

private:
    Ui::GlobalRTPCParamsWidget *ui;
    AudioControls::TConnectionPtr m_connection;
    bool m_inUpdateWidgetsFromConnection = false;

    void UpdateWidgetsFromConnection();

};

#endif // GLOBALRTPCPARAMSWIDGET_H
