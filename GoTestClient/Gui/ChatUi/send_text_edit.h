#ifndef SENDTEXTEDIT_H
#define SENDTEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QTextCursor>
#include <QTextImageFormat>
#include <QKeyEvent>
#include <QLabel>
#include <QMovie>
#include <QApplication>
#include <QClipboard>
#include <QTextDocumentFragment>
#include <QMimeData>
#include "Network/packet_define.h"

class SendTextEdit: public QTextEdit
{
    Q_OBJECT
public:
    SendTextEdit(QWidget *parent = nullptr);
    /**
      插入表情
    */
    void insertEmoji(int index);

    QString getMsg();
signals:
    void signSendMsg(QString msg);
protected slots:
    void keyPressEvent(QKeyEvent *event) override;

    void handleTextChanged();

    void mouseDoubleClickEvent(QMouseEvent *event) override ;
private slots:
    void addAnimation(const QString& url, const QString& fileName);

private slots:
    void subAnimate(int);

private:
    QList<QString> m_lstUrls;
    QHash<QMovie*, QString> m_hasUrls;
private:

    QString replaceEmojisImgs();
};

#endif // SENDTEXTEDIT_H
