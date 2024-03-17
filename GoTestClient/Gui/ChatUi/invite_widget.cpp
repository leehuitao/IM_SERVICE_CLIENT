#include "invite_widget.h"
#include "ui_invite_widget.h"

InviteWidget::InviteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InviteWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint); // 设置窗口为无边框
    init();
}

InviteWidget::~InviteWidget()
{
    delete ui;
}

void InviteWidget::init()
{
    // 获取屏幕的尺寸
    QRect screenGeometry = QApplication::primaryScreen()->geometry();

    int taskbarHeight = 60; // 假设任务栏高度为60px
    int margin = 5; // 窗口与屏幕边缘的间距

    int x = screenGeometry.width() - this->width() - margin;
    int y = screenGeometry.height() - this->height() - taskbarHeight - margin;

    // 将窗口移动到屏幕的右下角
    this->move(x, y);
}

void InviteWidget::setStatusInfo(const QString &statusInfo)
{
    ui->status_label->setText(statusInfo);
}

void InviteWidget::on_hang_up_toolBtn_clicked()
{
    signHungUp(m_msyBody, m_type);
    close();
}


void InviteWidget::on_listen_toolBtn_clicked()
{
    signListen(m_msyBody, m_type);
    close();
}

