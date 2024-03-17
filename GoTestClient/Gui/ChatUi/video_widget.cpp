#include "video_widget.h"
#include "ui_video_widget.h"

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoWidget)
{
    ui->setupUi(this);
}

VideoWidget::~VideoWidget()
{
    delete ui;
}

void VideoWidget::init()
{
    ui->local_label->hide();
}

void VideoWidget::setHeadUrl(const QPixmap &pixmap)
{
    ui->head_label->setPixmap(pixmap);
}

void VideoWidget::setNickNameInfo(const QString& nameInfo)
{

    ui->nickname_label->setText(nameInfo);

}

void VideoWidget::setStatusInfo(const QString& statusInfo)
{
    if(statusInfo.isEmpty())
    {
        ui->status_label->hide();
        ui->nickname_label->hide();
        ui->head_label->hide();
        ui->remote_label->clear();
    }
    else
    {
        ui->status_label->show();
        ui->status_label->setText(statusInfo);
    }

}

void VideoWidget::setRemoteLabelHide(bool status)
{
    if(status)
        ui->local_label->show();
    else
        ui->local_label->hide();
}

void VideoWidget::paintRemoteVideo(const QImage &img)
{
    if(m_connceted)
    {
        ui->remote_label->setPixmap(QPixmap::fromImage(img));
    }
}

void VideoWidget::paintLocalVideo(const QImage &img)
{
    if(!m_connceted)
    {
        ui->remote_label->setPixmap(QPixmap::fromImage(img));
    }
    else
    {
        ui->local_label->setPixmap(QPixmap::fromImage(img));
    }
}


void VideoWidget::on_call_toolBtn_clicked()
{
    close();
}

void VideoWidget::closeEvent(QCloseEvent *event)
{
    if(m_sendCloseSign)
        signCloseVideo();
}
