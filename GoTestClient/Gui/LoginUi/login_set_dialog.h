#ifndef LOGIN_SET_DIALOG_H
#define LOGIN_SET_DIALOG_H

#include <QDialog>

namespace Ui {
class LoginSetDialog;
}

class LoginSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginSetDialog(QWidget *parent = nullptr);
    ~LoginSetDialog();

    void init(QString ip , int port);

protected:
    virtual void accept();
    virtual void reject();
private:
    Ui::LoginSetDialog *ui;
};

#endif // LOGIN_SET_DIALOG_H
