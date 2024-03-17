#include "head_dialog.h"
#include "ui_head_dialog.h"

HeadDialog::HeadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeadDialog)
{
    ui->setupUi(this);

    fadeTimer = new QTimer(this);
    enableTimer = new QTimer(this);
    connect(fadeTimer, &QTimer::timeout, this, &HeadDialog::fadeDialog);
    connect(enableTimer, &QTimer::timeout, [=]{
        fadeTimer->start(100);
    });


}

HeadDialog::~HeadDialog()
{
    delete ui;
}

void HeadDialog::enterEvent(QEvent *event)
{
    isIn = 1;
    setWindowOpacity(1.0);
    fadeTimer->stop();
    enableTimer->stop();

}

void HeadDialog::leaveEvent(QEvent *event)
{
    isIn =0;
    enableTimer->start(1000);
}

