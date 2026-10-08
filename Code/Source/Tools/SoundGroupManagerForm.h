#ifndef SOUNDGROUPMANAGERFORM_H
#define SOUNDGROUPMANAGERFORM_H

#include <QWidget>

namespace Ui {
class SoundGroupManagerForm;
}

class SoundGroupManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit SoundGroupManagerForm(QWidget *parent = nullptr);
    ~SoundGroupManagerForm();

private:
    Ui::SoundGroupManagerForm *ui;
};

#endif // SOUNDGROUPMANAGERFORM_H
