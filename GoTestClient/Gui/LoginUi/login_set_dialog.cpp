#include "login_set_dialog.h"
#include "ui_login_set_dialog.h"
#include "Logic/global_config.h"
LoginSetDialog::LoginSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginSetDialog)
{
    ui->setupUi(this);
}

LoginSetDialog::~LoginSetDialog()
{
    delete ui;
}

void LoginSetDialog::init(QString ip, int port)
{
    ui->ip_lineEdit->setText(ip);

    ui->port_lineEdit->setText(QString::number(port));
}

void LoginSetDialog::accept()
{
    auto ip = ui->ip_lineEdit->text();
    auto port = ui->port_lineEdit->text().toInt();
    GlobalConfig::getInstance()->setKeyValue("CONNECT","ip",ip);
    GlobalConfig::getInstance()->setKeyValue("CONNECT","port",port);
    hide();
}

void LoginSetDialog::reject()
{
    hide();
};
