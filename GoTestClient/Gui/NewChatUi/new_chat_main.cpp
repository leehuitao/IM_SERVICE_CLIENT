#include "new_chat_main.h"
#include "ui_new_chat_main.h"

#include <QDateTime>
NewChatMain::NewChatMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewChatMain)
{
    ui->setupUi(this);
}

NewChatMain::~NewChatMain()
{
    delete ui;
}

void NewChatMain::clear()
{
    ui->chat_listWidget->clear();
}


void NewChatMain::addNewMsg(const MsgBody &message)
{
    QString msg = message.Msg;
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    bool isSending = true; // 发送中

    qDebug()<<"addMessage" << msg << time << ui->chat_listWidget->count();
    //    if(ui->chat_listWidget->count()%2) {
    if(isSending) {

        QNChatMessage* messageW = new QNChatMessage(ui->chat_listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui->chat_listWidget);
        dealMessage(messageW, item,message);
    }
    ui->chat_listWidget->setCurrentRow(ui->chat_listWidget->count()-1);
}

void NewChatMain::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, const MsgBody &message)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(message.Msg);
    item->setSizeHint(size);
    messageW->setText(message, size);
    ui->chat_listWidget->setItemWidget(item, messageW);
}

void NewChatMain::dealMessageTime(QString curMsgTime)
{

}

void NewChatMain::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    for(int i = 0; i < ui->chat_listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->chat_listWidget->itemWidget(ui->chat_listWidget->item(i));
        QListWidgetItem* item = ui->chat_listWidget->item(i);

        dealMessage(messageW, item, messageW->getMsgBody());
    }
}

void NewChatMain::parseMsg(const QString &source)
{
    //    auto len = source.size();
    //    for (int i = 0; i < len; i++) {
    //        auto temp = source.mid(i);
    //        if (temp.startsWith(EmjHeader)) {
    //            // 处理表情标记
    //            int emjEndPos = temp.indexOf(EmjEnd);
    //            if (emjEndPos != -1) {
    //                QString emjText = temp.mid(10 , emjEndPos - 10); // 获取emj名
    //                qDebug()<<"emjText = "<<emjText;
    //                // 在这里处理表情文本 emjText
    //                i += emjText.size() + 10 + 7; // Move i to the position after the "%$emjend"
    //                // 将图像插入到文本编辑框
    ////                QTextImageFormat fragment;
    ////                fragment.setName(EmojiPath[emjText.toInt()]);
    ////                textCursor().insertImage(fragment);
    //            } else {
    //                // 没有找到匹配的"%$emjend"标记
    //                // 处理错误情况
    //            }
    //        } else if (temp.startsWith(ImageHeader)) {
    //            // 处理图片标记
    //            int imgEndPos = temp.indexOf(ImageEnd);
    //            if (imgEndPos != -1) {
    //                QString imgText = temp.mid(10, imgEndPos - 10); //获取图片名称
    //                // 在这里处理图片文本 imgText
    //                i += imgText.size()+ 10 + 7; // 移动到 "%$imgend" 后面
    //                // 将图像插入到文本编辑框
    //                QTextImageFormat fragment;
    //                auto imgPath = imgText.contains("/") ? imgText: AppCache::Instance()->m_fileSavePath + imgText;

    //                QImage temp(imgPath);
    //                if(temp.width() > 100){
    //                    temp = temp.scaledToWidth(100,Qt::SmoothTransformation);
    //                }
    //                if(temp.height()>100){
    //                    temp = temp.scaledToHeight(100,Qt::SmoothTransformation);
    //                }
    //                fragment.setName(imgPath);
    //                fragment.setWidth(temp.width()); // 设置图像宽度
    //                fragment.setHeight(temp.height()); // 设置图像高度
    //                textCursor().insertImage(fragment);
    //            } else {
    //                // 没有找到匹配的"%$imgend"标记
    //                // 处理错误情况
    //            }
    //        } else {
    //            // 普通文本字符
    //            this->insertPlainText(source.at(i));
    //        }
    //    }

}


void NewChatMain::updateMsgReadStatus(QString msgId, int status)
{
    QList<QNChatMessage*> tlist = ui->chat_listWidget->findChildren<QNChatMessage*>();

    for(auto it : tlist){
        if(it->getMsgId() == msgId){
            it->updateMsgReadStatus(status);
        }
    }
}
