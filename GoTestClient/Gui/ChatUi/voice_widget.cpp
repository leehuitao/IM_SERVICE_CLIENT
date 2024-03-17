#include "voice_widget.h"
#include "ui_voice_widget.h"

VoiceWidget::VoiceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VoiceWidget)
{
    ui->setupUi(this);
    init();
}

VoiceWidget::~VoiceWidget()
{
    delete ui;
}

void VoiceWidget::init()
{
//    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
//    effect->setBlurRadius(10); // 设置虚化半径
//    this->setGraphicsEffect(effect);
}

void VoiceWidget::setHeadUrl(const QPixmap &pixmap)
{
    ui->head_label->setPixmap(pixmap);
}

void VoiceWidget::setNickNameInfo(const QString& nameInfo)
{

    ui->nickname_label->setText(nameInfo);

}

void VoiceWidget::setStatusInfo(const QString& statusInfo)
{
    if(statusInfo.isEmpty())
    {
        ui->status_label->hide();
    }
    else
    {
        ui->status_label->show();
        ui->status_label->setText(statusInfo);
    }

}

void VoiceWidget::on_call_toolBtn_clicked()
{
    close();
}

void VoiceWidget::closeEvent(QCloseEvent *event)
{
    if(m_sendCloseSign)
        signCloseVoice();
}

