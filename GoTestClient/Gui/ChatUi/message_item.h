#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QObject>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextCursor>
#include <QMouseEvent>
#include <QUrl>
#include <QDebug>
#include <QPainter>
#include <QMovie>

#include "Network/packet_define.h"
class MessageItem: public QTextEdit
{
    Q_OBJECT
public:
    MessageItem(QWidget *parent = nullptr);

    void initMsg(const MsgBody &msg);
    void initMsg(const QString &imgPath);
    void wheelEvent(QWheelEvent *event) override {
            // 重写 wheelEvent，但不调用基类的实现
            // 这样做会禁止滚轮滚动
            // QTextEdit::wheelEvent(event); // 注释掉或移除此行
        }
//    void paintEvent(QPaintEvent *event) override ;

    virtual void resizeEvent(QResizeEvent *event);
signals:
    void signImageClicked(const QString & imagePath );
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override ;

    void parseMsg(const QString & msg);
    void addAnimation(const QString& url, const QString& fileName);

public slots:
    void subAnimate(int);

private:
    QList<QString> m_lstUrls;
    QHash<QMovie*, QString> m_hasUrls;
private:
    int m_lineHeight;
    MsgBody m_msgBody;

    bool updateBackground = 1;
};

#endif // MESSAGEITEM_H
