#include "head_widget.h"
#include "ui_head_widget.h"

HeadWidget::HeadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeadWidget)
{
    ui->setupUi(this);
    connect(&m_hideTimer,&QTimer::timeout,[=]{
        m_headDialog.fadeTimer->start(100);
    });
}

HeadWidget::~HeadWidget()
{
    delete ui;
}

bool HeadWidget::setHeadImage(QString path)
{
    ui->widget->setStyleSheet(QString("border-image: url(%1);").arg(path));

    return 1;
}

void HeadWidget::enterEvent(QEvent *event)
{
    this->setCursor(Qt::PointingHandCursor);
    m_isAnchro = true;
//    if(!m_headDialog.isVisible()){
//        QPoint globalPos = QCursor::pos();
//        m_headDialog.setWindowOpacity(1.0);
//        m_headDialog.move(globalPos);
//        m_headDialog.show();
//    }

}

void HeadWidget::leaveEvent(QEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    m_isAnchro = false;
//    if(!m_headDialog.isIn)
    //        m_hideTimer.start(1000);
}

void HeadWidget::mousePressEvent(QMouseEvent *ev)
{
    if(m_isAnchro)
    {
        if(!m_headDialog.isVisible()){
            QPoint globalPos = QCursor::pos();
            m_headDialog.setWindowOpacity(1.0);
            m_headDialog.move(globalPos);
            m_headDialog.show();
        }
    }
}

void HeadWidget::dialogHide()
{
    if(!m_headDialog.isHidden())
        m_headDialog.hide();
}
