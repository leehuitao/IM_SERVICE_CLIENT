#ifndef SETTING_DIALOG_H
#define SETTING_DIALOG_H

#include <QDialog>
#include "Network/packet_define.h"
namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    void init(LoginBody body);
    void updateHeadImage(QString path);
private slots:
    void on_change_head_clicked();

private:
    Ui::SettingDialog *ui;
};

#endif // SETTING_DIALOG_H
