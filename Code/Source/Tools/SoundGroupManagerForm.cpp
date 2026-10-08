#include "SoundGroupManagerForm.h"
#include "ui_SoundGroupManagerForm.h"

#include <AzCore/std/limits.h>
#include <AzQtComponents/Components/Widgets/CheckBox.h>

SoundGroupManagerForm::SoundGroupManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoundGroupManagerForm)
{
    ui->setupUi(this);
    ui->dsbPitch->setMaximum(AZStd::numeric_limits<double>::max());

    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chk_NoDefaultAttach);
    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chk_NoSpatial);
    AzQtComponents::CheckBox::applyToggleSwitchStyle(ui->chk_NoPitch);

}

SoundGroupManagerForm::~SoundGroupManagerForm()
{
    delete ui;
}
