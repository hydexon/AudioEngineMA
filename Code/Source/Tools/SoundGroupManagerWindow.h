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

    void onNewLayoutActionTriggered();
    void onLoadLayoutActionTriggered();
    void onSaveLayoutActionTriggered();
    void onSaveLayoutAsActionTriggered();

private:
    QString m_currentLayoutPath;
    bool m_loadLayoutWasRequested = false;

    void SetCurrentSavePath(const QString& path);
    void SaveLayoutFile(bool saveAs);
    void ResetSelectionState();
    void PopulateChildItems(QTreeWidgetItem* parent, AZStd::vector<AudioEngineMA::SoundGroupData>& data);

    //SoundGroupManagerNotificationBus
    void OnRequestCompleted_AddSoundGroup(bool success, AudioEngineMA::SoundGroupData data, AZ::Name parentGroup) override;
    void OnRequestCompleted_RemoveSoundGroup(bool success, AZ::Name soundGroupName) override;
    void OnRequestCompleted_RenameSoundGroup(bool success, AZ::Name oldGroupName, AZ::Name newGroupName) override;
    void OnUpdateSoundGroupLayout(AudioEngineMA::SoundGroupLayoutData sndGroupLayout) override;

    Ui::SoundGroupManagerWindow *ui;
};

#endif // SOUNDGROUPMANAGERWINDOW_H
