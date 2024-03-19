#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDateTime>
#include <QFileDialog>
#include <QNetworkInterface>
#include <QScrollBar>
#include <QThread>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QKeyEvent>
#include <QPainter>
#include <QTextDocumentFragment>
#include <QMimeData>
#include <QUuid>
#include "Logic/global_center.h"
#include "Gui/ChatUi/msg_widget_item.h"
#include "Logic/global_ui_center.h"
#include <QPropertyAnimation>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("超信");
    setWindowIcon(QIcon(":/resource/self.png"));

    GlobalUiCenter::getInstance()->setMainWindow(this);
    GlobalUiCenter::getInstance()->setHeadWidget(ui->header_widget);
    GlobalUiCenter::getInstance()->setSendTextEdit(ui->msg_send_edit);
    GlobalUiCenter::getInstance()->setOrgMainWidget(ui->org_widget);
    GlobalUiCenter::getInstance()->setMessageInterfaceLayout(ui->history_layout);
    GlobalUiCenter::getInstance()->setHistoricalUserList(ui->msg_listWidget);
    GlobalUiCenter::getInstance()->setFileGridLayout(ui->gridLayout);
    GlobalUiCenter::getInstance()->setStackedWidget(ui->stackedWidget);
    GlobalUiCenter::getInstance()->setGroupWidget(ui->group_widget);
    GlobalUiCenter::getInstance()->initUi();

    this->setMinimumWidth(1080);

    m_emojiWidget = new EmotionWidget;
    m_emojiWidget->initEmotion();
    connect(m_emojiWidget ,&EmotionWidget::emojiWidgetClose, [=]{
        ui->msg_send_edit->setFocus();
    });

    connect(m_emojiWidget,&EmotionWidget::emojiClicked,[=](int number){
        ui->msg_send_edit->insertEmoji(number);
    });

    connect(ui->org_widget,&OrgMainWidget::signLogout,this,&MainWindow::signLogout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->msg_send_edit)		//可替换
    {
        if(event->type() == QEvent::KeyPress )//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);

            if(k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter)
            {
                on_sendmsg_btn_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

void MainWindow::on_sendmsg_btn_clicked()
{
    GlobalUiCenter::getInstance()->on_sendmsg_btn_clicked();
}

void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    //    qDebug()<<__FUNCTION__<<currentText<<" clicked ";
    //    GlobalCenter::getInstance()->setCurrentUserName(currentText);
}

void MainWindow::on_emoji_btn_clicked()
{
    if(!m_emojiWidget->isVisible()){
        m_emojiWidget->move(ui->emoji_btn->x()+this->x()-(m_emojiWidget->width()*0.5),ui->emoji_btn->y()+this->y()-m_emojiWidget->height()+23);

        m_emojiWidget->show();
    }else{
        m_emojiWidget->hide();
    }

}

void MainWindow::on_status_comboBox_currentIndexChanged(int index)
{
    LoginBody body;
    body.UserId = AppCache::Instance()->m_userId;
    body.UserLoginName   = AppCache::Instance()->m_userName;
    body.UserName   = AppCache::Instance()->m_userName;
    body.LoginStatus = index +1;
    GlobalCenter::getInstance()->signUpdateStatus(body,UpdateUserStatus,0);

}

void MainWindow::on_screen_btn_clicked()
{
    if(ScreenWidget::Instance()->btn != nullptr)
    {
        ScreenWidget::Instance()->btn->hide();
        ScreenWidget::Instance()->btn = nullptr;
    }
    ScreenWidget::Instance()->showFullScreen();
}

void MainWindow::on_screen_noself_btn_clicked()
{
    this->showMinimized();
    _sleep(500);
    if(ScreenWidget::Instance()->btn != nullptr)
    {
        ScreenWidget::Instance()->btn->hide();
        ScreenWidget::Instance()->btn = nullptr;
    }
    ScreenWidget::Instance()->showFullScreen();
    this->showNormal();
}

void MainWindow::on_file_btn_clicked()
{
    GlobalUiCenter::getInstance()->on_file_btn_clicked();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

}

void MainWindow::on_msg_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_org_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_header_btn_clicked()
{

}

void MainWindow::on_group_btn_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_set_btn_clicked()
{
    GlobalUiCenter::getInstance()->on_set_btn_clicked();
}

void MainWindow::on_msg_listWidget_itemClicked(QListWidgetItem *item)
{
    GlobalUiCenter::getInstance()->on_msg_listWidget_itemClicked(item);
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    ui->header_widget->dialogHide();
}

void MainWindow::shakeWindow() {

    setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    activateWindow();
    raise();
    activateWindow();
    // 创建动画对象，指定要动画化的属性为几何属性
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

    // 设置动画持续时间
    animation->setDuration(500);

    // 获取当前窗口位置和大小
    QRect geometry = this->geometry();
    int x = geometry.x();
    int y = geometry.y();
    int w = geometry.width();
    int h = geometry.height();

    // 设置动画路径，通过改变位置实现抖动
    animation->setKeyValueAt(0, QRect(x, y, w, h));
    animation->setKeyValueAt(0.1, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.2, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.3, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.4, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.5, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.6, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.7, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(0.8, QRect(x - 5, y + 5, w, h));
    animation->setKeyValueAt(0.9, QRect(x + 5, y - 5, w, h));
    animation->setKeyValueAt(1, QRect(x, y, w, h));

    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::on_test_btn_clicked()
{
    shakeWindow();
}

void MainWindow::on_jitter_btn_clicked()
{
    shakeWindow();
    GlobalCenter::getInstance()->slotJitter();
//    ui->history_wid->addShakeMsg(ShakeSend);
}

void MainWindow::on_audio_btn_clicked()
{
    GlobalUiCenter::getInstance()->on_audio_btn_clicked();
}

void MainWindow::on_video_btn_clicked()
{
    GlobalUiCenter::getInstance()->on_video_btn_clicked();
}

