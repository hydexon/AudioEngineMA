#ifndef SOUNDGROUPMANAGERWINDOW_H
#define SOUNDGROUPMANAGERWINDOW_H

#include <QMainWindow>
#include <EditorEngineInterop.h>

class QTreeWidgetItem;

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
    void onRenameGroupClicked();
    void onClearSelectionClicked();
    void onSoundGroupItemSelected(QTreeWidgetItem* current, QTreeWidgetItem* previous);

private:
    void OnRequestCompleted_AddSoundGroup(bool success, AudioEngineMA::SoundGroupData data, AZ::Name parentGroup) override;
    void OnRequestCompleted_RemoveSoundGroup(bool success, AZ::Name soundGroupName) override;
    void OnRequestCompleted_RenameSoundGroup(bool success, AZ::Name oldGroupName, AZ::Name newGroupName) override;

    Ui::SoundGroupManagerWindow *ui;
};

#endif // SOUNDGROUPMANAGERWINDOW_H
