#ifndef SYSTEM_SET_DIALOG_H
#define SYSTEM_SET_DIALOG_H

#include <QDialog>

namespace Ui {
class SystemSetDialog;
}

class SystemSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemSetDialog(QWidget *parent = nullptr);
    ~SystemSetDialog();

    virtual void accept() override;
private:
    Ui::SystemSetDialog *ui;
};

#endif // SYSTEM_SET_DIALOG_H
