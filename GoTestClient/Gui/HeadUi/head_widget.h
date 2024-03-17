#ifndef HEAD_WIDGET_H
#define HEAD_WIDGET_H

#include <QWidget>
#include "head_dialog.h"
#include <QTimer>
namespace Ui {
class HeadWidget;
}

class HeadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeadWidget(QWidget *parent = nullptr);
    ~HeadWidget();

    bool setHeadImage(QString path);

    virtual void enterEvent(QEvent *event);

    virtual void leaveEvent(QEvent *event);

    virtual void mousePressEvent(QMouseEvent *ev);

    void dialogHide();

private:
    Ui::HeadWidget *ui;
    HeadDialog m_headDialog;

    QTimer m_hideTimer;

    bool m_isAnchro;
};

#endif // HEAD_WIDGET_H
