#ifndef AUDIOFILETOTRIGGERPARAMSWIDGET_H
#define AUDIOFILETOTRIGGERPARAMSWIDGET_H

#include <QWidget>
#include <ACETypes.h>

namespace Ui {
class AudioFileToTriggerParamsWidget;
}

class AudioFileToTriggerParamsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioFileToTriggerParamsWidget(AudioControls::TConnectionPtr connection, QWidget *parent = nullptr);
    ~AudioFileToTriggerParamsWidget();

signals:
    void PropertiesChanged();

private slots:
    void OnSoundActionChanged(int index);
    void UpdateConnectionFromWidgets();

private:
    Ui::AudioFileToTriggerParamsWidget *ui;
    AudioControls::TConnectionPtr m_connection;
    bool m_inUpdateWidgetsFromConnection = false;

    void UpdateWidgetsFromConnection();
};

#endif // AUDIOFILETOTRIGGERPARAMSWIDGET_H
