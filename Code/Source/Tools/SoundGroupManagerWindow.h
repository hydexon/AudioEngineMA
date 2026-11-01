#ifndef SOUNDGROUPMANAGERWINDOW_H
#define SOUNDGROUPMANAGERWINDOW_H

#include <QMainWindow>

namespace Ui {
class SoundGroupManagerWindow;
}

class SoundGroupManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SoundGroupManagerWindow(QWidget *parent = nullptr);
    ~SoundGroupManagerWindow();

private slots:
    void onAddGroupClicked();
    void onRemoveGroupClicked();

private:
    Ui::SoundGroupManagerWindow *ui;
};

#endif // SOUNDGROUPMANAGERWINDOW_H
