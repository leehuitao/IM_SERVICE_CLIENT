#include "message_box_item.h"
#include "ui_message_box_item.h"

#include <QDesktopServices>
#include <QProcess>
#include <QRegularExpression>
#include "Logic/app_cache.h"
#include "img_label.h"

MessageBoxItem::MessageBoxItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageBoxItem)
{
    ui->setupUi(this);
    ui->other_image_label->hide();
    ui->self_image_label->hide();

    QObject::connect(ui->other_image_label, &ImgLabel::clicked, [&](){
        QDialog *dialog = new QDialog();
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        QLabel *imageLabel = new QLabel(dialog);

        // 设置图片，并保持图片的宽高比
        imageLabel->setPixmap(m_imgPath);
//        imageLabel->setScaledContents(true); // 让图片填充整个 QLabel

        // 设置 QLabel 的对齐方式为居中
        imageLabel->setAlignment(Qt::AlignCenter);

        // 添加 QLabel 到布局并设置布局
        layout->addWidget(imageLabel);
        dialog->setLayout(layout);

        // 显示全屏对话框
        dialog->showFullScreen();
    });
    QObject::connect(ui->self_image_label, &ImgLabel::clicked, [&](){
        QDialog *dialog = new QDialog();
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        QLabel *imageLabel = new QLabel(dialog);

        // 设置图片，并保持图片的宽高比
        imageLabel->setPixmap(m_imgPath);
//        imageLabel->setScaledContents(true); // 让图片填充整个 QLabel

        // 设置 QLabel 的对齐方式为居中
        imageLabel->setAlignment(Qt::AlignCenter);

        // 添加 QLabel 到布局并设置布局
        layout->addWidget(imageLabel);
        dialog->setLayout(layout);

        // 显示全屏对话框
        dialog->showFullScreen();
    });
}

MessageBoxItem::~MessageBoxItem()
{
    delete ui;
}

void MessageBoxItem::updateMsgReadStatus(int status)
{
    m_currentState = status;
    if(status == 1){
        ui->self_msg_state_lab->setText("未读");
    }else if(status == 2){
        ui->self_msg_state_lab->setText("已读");
    }else{
        ui->self_msg_state_lab->setText("错误");
    }
}

void MessageBoxItem::resizeEvent(QResizeEvent *event)
{
    if(!isImageMessage)
        initMsg(m_msgBody,0);
}

void MessageBoxItem::initMsg(const QString &filePath, int type)
{
    m_filePath = filePath;
    if(type == 0){
        setDisPlayWid(3);
        ui->send_file_name_lab->setText(filePath.split("/").last());
        m_size = QSize(this->width(),50);
    }else if(type == 1){
        setDisPlayWid(4);
        ui->recv_file_name_lab->setText(filePath.split("/").last());
        m_size = QSize(this->width(),50);
    }else if(type == 5){
        setDisPlayWid(5);
        ui->shake_lab->setText(filePath);
        m_size = QSize(this->width(),50);
    }
}

void MessageBoxItem::initMsg(const MsgBody &msg, bool updateMsg)
{
    m_msgBody = msg;

    m_msgId = m_msgBody.MsgId;
    //绘制用户头像
    setUserAvatar(m_msgBody.SendUserId);
    //是我发送的消息
    //UI总宽度
    auto widWidth = this->width();
    auto widheight = this->height();
    //可用长度   行最大长度
    widWidth = widWidth - (40 + 19) *2 - 60;

    QFontMetricsF fm(this->font());
    // 使用正则表达式匹配模式
    QRegularExpression regex("%\\$emjstart.*?%\\$emjend");

    // 替换文本
    QString replaceText = "表情"; // 您要替换的表情或文本

    QString imgStart = ImageHeader;
    QString imgEnd = ImageEnd;

    auto src = m_msgBody.Msg;
    src = src.replace(regex, replaceText);
    //文字总长度
    int nMaxWidth = 0;
    nMaxWidth = fm.width(src);
    auto msgBody = m_msgBody;
    if(m_msgBody.SendUserId == AppCache::Instance()->m_userId){
        // 设置对齐方式
        setDisPlayWid(1);
        ui->self_textEdit->setAlignment(Qt::AlignLeft); // 右对齐
        if(updateMsg)
            ui->self_textEdit->initMsg(msgBody);

        if(nMaxWidth <= widWidth){
            ui->self_textEdit->setFixedWidth(fm.width(src) + 20);
            ui->self_textEdit->setFixedHeight(fm.height() + 16 > 40 ? fm.height() + 16 : 40 );
            m_size = QSize(ui->self_textEdit->width() + 60,ui->self_textEdit->height()+ 18);
        }else{
            ui->self_textEdit->setFixedWidth(widWidth);
            auto h =  ((nMaxWidth / widWidth) + 1)* (fm.lineSpacing() + 2)+5 ;
            ui->self_textEdit->setFixedHeight(h);
            m_size = QSize(this->width(),h+18);
        }
        if(m_currentState > m_msgBody.MsgStatus)
            m_msgBody.MsgStatus = m_currentState;
        if(m_msgBody.MsgStatus == 0){//发送中
            ui->self_msg_state_lab->setText("发送中");
        }else if(m_msgBody.MsgStatus == 1){//未读
            ui->self_msg_state_lab->setText("未读");
        }else if(m_msgBody.MsgStatus == 2){//已读
            ui->self_msg_state_lab->setText("已读");
        }else{//错误
            return;
        }
    }else if(m_msgBody.DstUserId == AppCache::Instance()->m_userId){//是我接收的消息
        if(nMaxWidth <= widWidth){
            ui->other_textEdit->setFixedWidth(fm.width(src) + 20);
            ui->other_textEdit->setFixedHeight(fm.height() + 16 > 40 ? fm.height() + 16 : 40);
            m_size = QSize(ui->other_textEdit->width() + 60,ui->other_textEdit->height()+ 18);
        }else{
            ui->other_textEdit->setFixedWidth(widWidth);

            auto h =  ((nMaxWidth / widWidth) + 1)* (fm.lineSpacing() + 2) + 5;
            ui->other_textEdit->setFixedHeight(h);
            m_size = QSize(this->width(),h+18);
        }
        // 设置对齐方式
        setDisPlayWid(2);
        ui->other_textEdit->setAlignment(Qt::AlignLeft); // 左对齐
        if(updateMsg)
            ui->other_textEdit->initMsg(msgBody);

    }else{//用户ID错误
        return;
    }

}

void MessageBoxItem::initGroupMsg(const GroupBody &msg, bool updateMsg)
{
    m_groupMsgBody = msg;

    m_msgId = m_groupMsgBody.MsgId;
    //绘制用户头像
    setUserAvatar(m_groupMsgBody.SendUserId);
    //是我发送的消息
    //UI总宽度
    auto widWidth = this->width();
    auto widheight = this->height();
    //可用长度   行最大长度
    widWidth = widWidth - (40 + 19) *2 - 60;

    QFontMetricsF fm(this->font());
    // 使用正则表达式匹配模式
    QRegularExpression regex("%\\$emjstart.*?%\\$emjend");

    // 替换文本
    QString replaceText = "表情"; // 您要替换的表情或文本

    QString imgStart = ImageHeader;
    QString imgEnd = ImageEnd;

    auto src = m_groupMsgBody.Msg;
    src = src.replace(regex, replaceText);
    //文字总长度
    int nMaxWidth = 0;
    nMaxWidth = fm.width(src);
    if(m_groupMsgBody.SendUserId == AppCache::Instance()->m_userId){
        // 设置对齐方式
        setDisPlayWid(1);
        ui->self_textEdit->setAlignment(Qt::AlignLeft); // 右对齐

        ui->self_msg_state_lab->hide();
        if(nMaxWidth <= widWidth){
            ui->self_textEdit->setFixedWidth(nMaxWidth + 20);
            ui->self_textEdit->setFixedHeight(fm.height() + 16 > 40 ? fm.height() + 16 : 40 );
            m_size = QSize(ui->self_textEdit->width() + 60,ui->self_textEdit->height()+ 18);
        }else{
            ui->self_textEdit->setFixedWidth(widWidth);
            auto h =  ((nMaxWidth / widWidth) + 1)* (fm.lineSpacing() + 2)+5 ;
            ui->self_textEdit->setFixedHeight(h);
            m_size = QSize(this->width(),h+18);
        }
        if(updateMsg)
            ui->self_textEdit->initGroupMsg(m_groupMsgBody);
    }else if(m_groupMsgBody.SendUserId != AppCache::Instance()->m_userId){//是我接收的消息
        if(nMaxWidth <= widWidth){
            ui->other_textEdit->setFixedWidth(fm.width(src) + 20);
            ui->other_textEdit->setFixedHeight(fm.height() + 16 > 40 ? fm.height() + 16 : 40);
            m_size = QSize(ui->other_textEdit->width() + 60,ui->other_textEdit->height()+ 18);
        }else{
            ui->other_textEdit->setFixedWidth(widWidth);

            auto h =  ((nMaxWidth / widWidth) + 1)* (fm.lineSpacing() + 2) + 5;
            ui->other_textEdit->setFixedHeight(h);
            m_size = QSize(this->width(),h+18);
        }
        // 设置对齐方式
        setDisPlayWid(2);
        ui->other_textEdit->setAlignment(Qt::AlignLeft); // 左对齐
        if(updateMsg)
            ui->other_textEdit->initGroupMsg(m_groupMsgBody);

    }else{//用户ID错误
        return;
    }
}

void MessageBoxItem::initImageMsg(const MsgBody &msg, bool updateMsg)
{
    isImageMessage = 1;

    m_msgBody = msg;

    m_msgId = m_msgBody.MsgId;
    //绘制用户头像
    setUserAvatar(m_msgBody.SendUserId);
    m_imgPath = ChatImagePath+ msg.Msg.split("/").last();
    QImage temp( m_imgPath);
    if(temp.width() > 200){
        temp = temp.scaledToWidth(200,Qt::SmoothTransformation);
    }
    if(temp.height()>200){
        temp = temp.scaledToHeight(200,Qt::SmoothTransformation);
    }
    if(m_msgBody.SendUserId == AppCache::Instance()->m_userId){
        // 设置对齐方式
        setDisPlayWid(1);
        ui->self_textEdit->hide();
        ui->self_image_label->show();
        ui->self_image_label->setFixedWidth(temp.width());
        ui->self_image_label->setFixedHeight(temp.height());
        m_size = QSize(ui->self_image_label->width()+ 20,ui->self_image_label->height() + 40);
        ui->self_image_label->setPixmap(QPixmap::fromImage(temp));
        if(m_currentState > m_msgBody.MsgStatus)
            m_msgBody.MsgStatus = m_currentState;
        if(m_msgBody.MsgStatus == 0){//发送中
            ui->self_msg_state_lab->setText("发送中");
        }else if(m_msgBody.MsgStatus == 1){//未读
            ui->self_msg_state_lab->setText("未读");
        }else if(m_msgBody.MsgStatus == 2){//已读
            ui->self_msg_state_lab->setText("已读");
        }else{//错误
            return;
        }
    }else if(m_msgBody.DstUserId == AppCache::Instance()->m_userId){//是我接收的消息
        // 设置对齐方式
        setDisPlayWid(2);
        ui->other_textEdit->hide();
        ui->other_image_label->show();
        ui->other_image_label->setFixedWidth(temp.width());
        ui->other_image_label->setFixedHeight(temp.height());
        m_size = QSize(ui->other_image_label->width()+ 20,ui->other_image_label->height()+ 40);
        ui->other_image_label->setPixmap(QPixmap::fromImage(temp));
    }else{//用户ID错误
        return;
    }
}

void MessageBoxItem::initGroupImageMsg(const GroupBody &msg, bool updateMsg)
{
    isImageMessage = 1;

    m_groupMsgBody = msg;

    m_msgId = m_groupMsgBody.MsgId;
    //绘制用户头像
    setUserAvatar(m_groupMsgBody.SendUserId);
    m_imgPath = ChatImagePath+ msg.Msg.split("/").last();
    QImage temp( m_imgPath);
    if(temp.width() > 200){
        temp = temp.scaledToWidth(200,Qt::SmoothTransformation);
    }
    if(temp.height()>200){
        temp = temp.scaledToHeight(200,Qt::SmoothTransformation);
    }
    if(m_groupMsgBody.SendUserId == AppCache::Instance()->m_userId){
        // 设置对齐方式
        setDisPlayWid(1);
        ui->self_textEdit->hide();
        ui->self_image_label->show();
        ui->self_image_label->setFixedWidth(temp.width());
        ui->self_image_label->setFixedHeight(temp.height());
        m_size = QSize(ui->self_image_label->width()+ 20,ui->self_image_label->height() + 40);
        ui->self_image_label->setPixmap(QPixmap::fromImage(temp));

    }else if(m_groupMsgBody.SendUserId != AppCache::Instance()->m_userId){//是我接收的消息
        // 设置对齐方式
        setDisPlayWid(2);
        ui->other_textEdit->hide();
        ui->other_image_label->show();
        ui->other_image_label->setFixedWidth(temp.width());
        ui->other_image_label->setFixedHeight(temp.height());
        m_size = QSize(ui->other_image_label->width()+ 20,ui->other_image_label->height()+ 40);
        ui->other_image_label->setPixmap(QPixmap::fromImage(temp));
    }else{//用户ID错误
        return;
    }
}


void MessageBoxItem::on_open_file_btn_clicked()
{
    // 使用QDesktopServices打开文件
    QUrl fileUrl = QUrl::fromLocalFile(m_filePath);
    if (QDesktopServices::openUrl(fileUrl)) {
        // 文件成功打开
    } else {
        // 处理打开文件失败
    }

}

void MessageBoxItem::on_open_dir_btn_clicked()
{
    QProcess process;
    QString paht = m_filePath.replace("/", "\\"); // 只能识别 "\"
    QString cmd = QString("explorer.exe /select,\"%1\"").arg(paht);
    qDebug() << cmd;
    process.startDetached(cmd);

}

void MessageBoxItem::setDisPlayWid(int index)
{
    if(index == 1){//我发送的
        ui->self_wid->show();
        ui->other_wid->hide();
        ui->recv_file_wid->hide();
        ui->send_file_wid->hide();
        ui->shake_wid->hide();
    }else if(index == 2){//我接收的
        ui->shake_wid->hide();
        ui->self_wid->hide();
        ui->other_wid->show();
        ui->recv_file_wid->hide();
        ui->send_file_wid->hide();
    }else if(index == 3){//发送文件成功
        ui->self_wid->hide();
        ui->other_wid->hide();
        ui->recv_file_wid->hide();
        ui->send_file_wid->show();
        ui->shake_wid->hide();
    }else if(index == 4){//接收文件成功
        ui->self_wid->hide();
        ui->other_wid->hide();
        ui->recv_file_wid->show();
        ui->send_file_wid->hide();
        ui->shake_wid->hide();
    }else if(index == 5){//抖动消息
        ui->shake_wid->show();
        ui->self_wid->hide();
        ui->other_wid->hide();
        ui->recv_file_wid->hide();
        ui->send_file_wid->hide();
    }
}

void MessageBoxItem::setUserAvatar(int userId)
{
    auto path = AppCache::Instance()->m_headImagePath[userId];
    //    w->setUserInfo(userName,userId,path.isEmpty() ?m_headImagePath:path);
    if(userId == AppCache::Instance()->m_userId){//我发送的
        ui->self_icon_lab->setPixmap(QPixmap(path.isEmpty() ?"self.png":path));
    }else{//我接收的
        ui->other_icon_lab->setPixmap(QPixmap(path.isEmpty() ?"other.png":path));
    }
}
