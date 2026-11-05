#ifndef SOUNDGROUPMANAGERWINDOW_H
#define SOUNDGROUPMANAGERWINDOW_H

#include <QMainWindow>
#include <EditorEngineInterop.h>

namespace Ui {
class SoundGroupManagerWindow;
}

class SoundGroupManagerWindow
        : public QMainWindow
        , private AudioEngineMA::SoundGroupManagerNotificationBus::Handler
{
    Q_OBJECT

public:
    explicit SoundGroupManagerWindow(QWidget *parent = nullptr);
    ~SoundGroupManagerWindow();

private slots:
    void onAddGroupClicked();
    void onRemoveGroupClicked();

private:
    void OnRequestCompleted_AddSoundGroup(bool success, AudioEngineMA::SoundGroupData data, AZ::Name parentGroup) override;
    void OnRequestCompleted_RemoveSoundGroup(bool success, AZ::Name soundGroupName) override;
    void OnRequestCompleted_RenameSoundGroup(bool success, AZ::Name newGroupName) override;

    Ui::SoundGroupManagerWindow *ui;
};

#endif // SOUNDGROUPMANAGERWINDOW_H
