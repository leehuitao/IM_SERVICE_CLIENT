#ifndef IMGLABEL_H
#define IMGLABEL_H

#include <QLabel>
#include <QMouseEvent>

class ImgLabel : public QLabel {
    Q_OBJECT
public:
    explicit ImgLabel(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            // 当左键点击时
            emit clicked();
        }
    }

signals:
    void clicked(); // 点击信号
};

#endif // IMGLABEL_H
