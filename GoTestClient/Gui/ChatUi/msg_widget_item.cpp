#include "msg_widget_item.h"
#include "ui_msg_widget_item.h"

#include <QDateTime>
#include <QDebug>
#include <QPainter>
#include <QRegularExpression>
MsgWidgetItem::MsgWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgWidgetItem)
{
    ui->setupUi(this);
}

MsgWidgetItem::~MsgWidgetItem()
{
    delete ui;
}

void MsgWidgetItem::setUserInfo(QString name, int userid, QString avatarPath)
{
    m_userName = name;
    m_userid = userid;
    ui->username_lab->setText(name);
    m_avatarPath = avatarPath;
    m_primitive = QPixmap(avatarPath);
    ui->avatar_lab->setPixmap(m_primitive);
    ui->avatar_lab->setScaledContents(true); // 设置以缩放内容以填充QLabel
    ui->avatar_lab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置QLabel的大小策略
}

void MsgWidgetItem::setUserInfo(QString name, QString userid, QString avatarPath)
{
    m_groupName = name;
    m_groupid = userid;
    ui->username_lab->setText(name);
    m_avatarPath = avatarPath;
    m_primitive = QPixmap(avatarPath);
    ui->avatar_lab->setPixmap(m_primitive);
    ui->avatar_lab->setScaledContents(true); // 设置以缩放内容以填充QLabel
    ui->avatar_lab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置QLabel的大小策略
}

void MsgWidgetItem::setUserHead(QString avatarPath)
{
    m_avatarPath = avatarPath;
    m_primitive = QPixmap(avatarPath);
    ui->avatar_lab->setPixmap(m_primitive);
}

void MsgWidgetItem::updateMsgInfo(const QString &msg, const QString &time)
{
    QRegularExpression regex("%\\$emjstart.*?%\\$emjend");
    QRegularExpression regexImage("%\\$imgstart.*?%\\$imgend");

    // 替换文本
    QString replaceText = "[表情]"; // 您要替换的表情或文本
    QString replaceImage = "[图片]"; // 您要替换的表情或文本
    m_lastMsg = msg;
    m_lastMsg = m_lastMsg.replace(regex, replaceText);
    m_lastMsg = m_lastMsg.replace(regexImage, replaceImage);
    m_lastMsgTime = time.mid(0,16);
    QDateTime t = QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss");
    ui->lastmsg_lab->setText(msg);
    auto td = t.date().day();
    if( td != QDateTime::currentDateTime().date().day()){
        m_lastMsgTime = m_lastMsgTime.split(" ").first();
        ui->time_lab->setText(m_lastMsgTime);
    }else{
        m_lastMsgTime = m_lastMsgTime.split(" ").last();
        ui->time_lab->setText(m_lastMsgTime);
    }
    this->resizeEvent(nullptr);
}

void MsgWidgetItem::updateUnread(int number)
{
    // 创建一个 QPixmap 用于绘制
    QPixmap pixmap(ui->label->width(), ui->label->height()); // 与 QLabel 大小相同
    pixmap.fill(Qt::transparent); // 透明背景

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    if(number){
        // 绘制红色圆圈
        painter.setBrush(QBrush(Qt::red));
        painter.setPen(Qt::NoPen);
        int radius = ui->label->width()/2; // 圆的半径
        int x = pixmap.width() - radius - 10; // 圆的 x 坐标
        int y = pixmap.height() - radius - 10; // 圆的 y 坐标
        painter.drawEllipse(x, y, radius * 2, radius * 2);

        // 绘制白色数字
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(1);
        font.setPointSize(10);
        painter.setFont(font);
        QString text = QString::number(number);
        int textWidth = painter.fontMetrics().width(text);
        int textHeight = painter.fontMetrics().height();
        painter.drawText(x + (radius - textWidth / 2), y + (radius + textHeight / 4)+2, text);
    //    pixmap.save("C:/temp/ceshi.jpg");
    }

    // 将 QPixmap 设置到 QLabel 上
     ui->label->setPixmap(pixmap);
//    ui->label->setText(QString::number(number));
}

void MsgWidgetItem::clearUnread()
{
    ui->avatar_lab->setPixmap(m_primitive);
}

void MsgWidgetItem::resizeEvent(QResizeEvent *event)
{
    auto text = m_lastMsgTime;
    // 获取QLabel的字体度量，用于计算文本宽度
    //    QFontMetrics metrics(ui->time_lab->font());
    //    // 计算并设置省略后的文本
    //    int width = ui->time_lab->width(); // 获取QLabel的当前宽度
    //    QString elidedText = metrics.elidedText(text, Qt::ElideRight, width);
    ui->time_lab->setText(m_lastMsgTime);

    text = m_lastMsg;
    // 获取QLabel的字体度量，用于计算文本宽度
    QFontMetrics metrics1(ui->lastmsg_lab->font());

    // 计算并设置省略后的文本
    int width = ui->lastmsg_lab->width(); // 获取QLabel的当前宽度
    QString elidedText = metrics1.elidedText(text, Qt::ElideRight, width);
    ui->lastmsg_lab->setText(elidedText);
}
