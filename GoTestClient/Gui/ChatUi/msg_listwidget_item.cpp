#include "msg_listwidget_item.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

MsgListWidgetItem::MsgListWidgetItem(QWidget *parent):QWidget(parent)
{
    m_avatarLab = new QLabel;
    m_userNameLab = new QLabel;
    m_lastMsgLab = new QLabel;
    m_lastMsgTimeLab = new QLabel;

    // 创建一个水平布局
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    // 将两个横向排列的QLabel添加到水平布局
    horizontalLayout->addWidget(m_userNameLab);
    horizontalLayout->addWidget(m_lastMsgTimeLab);
    // 创建一个垂直布局
    QVBoxLayout *verticalLayout = new QVBoxLayout;

    // 将水平布局和垂直排列的QLabel添加到垂直布局
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(m_lastMsgLab);

    // 创建一个水平布局
    QHBoxLayout *horizontalLayoutMain = new QHBoxLayout;
    // 将两个横向排列的QLabel添加到水平布局
    horizontalLayoutMain->addWidget(m_avatarLab);
    horizontalLayoutMain->addLayout(verticalLayout);

    setLayout(horizontalLayoutMain);

    m_userNameLab->setText("ttttttttttt");
}

void MsgListWidgetItem::init(QString name, int userid, QString avatarPath)
{

}
