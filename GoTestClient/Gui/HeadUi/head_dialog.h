#ifndef HEAD_DIALOG_H
#define HEAD_DIALOG_H

#include <QDialog>
#include <QEvent>
#include <QTimer>

namespace Ui {
class HeadDialog;
}

class HeadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeadDialog(QWidget *parent = nullptr);
    ~HeadDialog();
    virtual void enterEvent(QEvent *event);

    virtual void leaveEvent(QEvent *event);

    bool isIn = false;

    void fadeDialog()
       {
           qreal opacity = windowOpacity();
           opacity -= 0.1; // 每次调用减少不透明度
           if (opacity <= 0) {
               fadeTimer->stop();
               close(); // 当对话框完全透明时关闭它
           } else {
               setWindowOpacity(opacity);
           }
       }

    QTimer *fadeTimer;
    QTimer *enableTimer;
private:
    Ui::HeadDialog *ui;
};

#endif // HEAD_DIALOG_H
