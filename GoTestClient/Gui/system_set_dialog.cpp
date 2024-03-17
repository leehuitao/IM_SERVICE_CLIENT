#include "system_set_dialog.h"
#include "ui_system_set_dialog.h"

SystemSetDialog::SystemSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemSetDialog)
{
    ui->setupUi(this);
}

SystemSetDialog::~SystemSetDialog()
{
    delete ui;
}

void SystemSetDialog::accept()
{

}
