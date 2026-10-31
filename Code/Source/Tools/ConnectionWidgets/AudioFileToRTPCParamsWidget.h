#ifndef AUDIOFILETORTPCPARAMSWIDGET_H
#define AUDIOFILETORTPCPARAMSWIDGET_H

#include <QWidget>
#include <ACETypes.h>

namespace Ui {
class AudioFileToRTPCParamsWidget;
}

class AudioFileToRTPCParamsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioFileToRTPCParamsWidget(AudioControls::TConnectionPtr connection, QWidget *parent = nullptr);
    ~AudioFileToRTPCParamsWidget();

signals:
    void PropertiesChanged();

private slots:
    void UpdateConnectionFromWidgets();

private:
    Ui::AudioFileToRTPCParamsWidget *ui;
    AudioControls::TConnectionPtr m_connection;
    bool m_inUpdateWidgetsFromConnection = false;

    void UpdateWidgetsFromConnection();

};

#endif // AUDIOFILETORTPCPARAMSWIDGET_H
