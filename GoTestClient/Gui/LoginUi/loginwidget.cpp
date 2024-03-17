#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "Logic/global_config.h"
#include <QDesktopServices>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("超信");
    setWindowIcon(QIcon(":/resource/self.png"));


    //    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(Qt::black);       //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(20);        //设定阴影的模糊半径，数值越大越模糊
    ui->frame->setGraphicsEffect(effect);
    ui->background_widget->setGraphicsEffect(effect);
    ui->frame->setGraphicsEffect(effect);


    //    this->resize(420,370);
    label = new QLabel(this);
    label->setText("信息有误");
    label->hide();
    //透明度
    goe = new QGraphicsOpacityEffect();
    label->setGraphicsEffect(goe);

    //移动动画
    animation = new QPropertyAnimation(label, "geometry");
    connect(animation,&QPropertyAnimation::finished,this,[=](){
        label->hide();
        timer->stop();
    });
    timer = new QTimer();
    //设置高精度定时器
    //timer->setTimerType(Qt::PreciseTimer);
    connect(timer,&QTimer::timeout,this,&LoginWidget::transparenceAnimation);

    initSysMenu();

    initView();

    auto user = GlobalConfig::getInstance()->getCurrentUserConfig();

    ui->account_comboBox->setCurrentText(user.userLoginName);
    ui->passwordEdit->setText(user.userPwd);

    ui->remember_checkBox->setChecked(user.rememberPwd);

    connect(&w,&MainWindow::signLogout,this,&LoginWidget::slotLogout);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signLoginStatus,         this,&LoginWidget::slotLoginStatus                 ,Qt::QueuedConnection);
    connect(GlobalCenter::getInstance() ,&GlobalCenter::signTpcConnected,         this,&LoginWidget::slotTcpConnected                 ,Qt::QueuedConnection);
    connect(&m_tcpTimer,&QTimer::timeout,[=]{
        m_tcpTimer.stop();
        QMessageBox::information(nullptr,QStringLiteral("错误"),QStringLiteral("远程服务器无法连接"));
//        ui->loginBtn->setEnabled(1);
    });
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::slotTcpConnected()
{
    m_tcpTimer.stop();
}

void LoginWidget::slotLogout()
{
    this->show();
}

void LoginWidget::on_loginBtn_clicked()
{
    QString account = ui->account_comboBox->currentText();
    QString password = ui->passwordEdit->text();
    GlobalConfig::getInstance()->setKeyValue("CurrentUser","userLoginName",account);
    GlobalConfig::getInstance()->setKeyValue("CurrentUser","userPwd",password);
    GlobalConfig::getInstance()->setKeyValue("CurrentUser","rememberPwd",ui->remember_checkBox->isChecked());
    GlobalConfig::getInstance()->setKeyValue("CurrentUser","post_loginStatus",1);
    bool ret = checkUser(account,password);
    if(ret)
    {
        AppCache::Instance()->m_userLoginName = account;
        AppCache::Instance()->m_passWord = password;

        //        w.show();
        //        w.on_login_btn_clicked();
        //        this->close();
        auto conf  = GlobalConfig::getInstance()->getConnectConfig();
        QString ip = conf.ip;
        int port   = conf.port;
        GlobalCenter::getInstance()->initTcp(ip,port);
        m_tcpTimer.start(2000);
//        ui->loginBtn->setDisabled(1);
        //启动登录动画
    }
    else
    {
        m_opacityVal = 1.0f;
        int t = 1500;//动画时间
        //移动动画
        animation->setDuration(t);//设置动画时间
        animation->setEndValue(QRect((this->width()-label->width())/2, 0, 150, 40));//开始位置
        animation->setStartValue(QRect((this->width()-label->width())/2, this->height()/2, 150, 40));//结束位置
        animation->start();//动画开始
        label->show();
        //        timer->start(t/10);
    }
}

void LoginWidget::on_registerBtn_clicked()
{
    const QUrl regUrl(QLatin1String("http://www.baidu.com"));
    QDesktopServices::openUrl(regUrl);
}

bool LoginWidget::checkUser(QString account, QString password)
{
    return 1;
}

void LoginWidget::transparenceAnimation()
{
    m_opacityVal -= 0.1;
    goe->setOpacity(m_opacityVal);
}
#include <QMouseEvent>

void LoginWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && event->y() < 90) {
        // 记录当前鼠标位置和窗口位置
        lastMousePos = event->globalPos();
        lastWindowPos = this->pos();
        dragging = true;
    }
}

void LoginWidget::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        // 计算鼠标的位移
        QPoint delta = event->globalPos() - lastMousePos;
        // 更新窗口位置
        move(this->pos() + delta);
        lastMousePos = event->globalPos();
    }
}

void LoginWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
    }
}


void LoginWidget::on_min_btn_clicked()
{
    this->showMinimized();
}

void LoginWidget::on_close_btn_clicked()
{
    toggleWindow();
}

void LoginWidget::toggleWindow()
{
    if (isVisible()) {
        hide();
    } else {
        show();
    }
}


void LoginWidget::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        toggleWindow();
    }
}

void LoginWidget::initSysMenu()
{
    // 创建系统托盘图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/resource/self.png"));

    // 创建托盘菜单
    trayMenu = new QMenu(this);
    QAction *toggleAction = new QAction("Toggle Window", this);
    connect(toggleAction, &QAction::triggered, this, &LoginWidget::toggleWindow);
    // 连接托盘图标的activated信号到槽函数
    connect(trayIcon, &QSystemTrayIcon::activated, this, &LoginWidget::trayIconActivated);
    trayMenu->addAction(toggleAction);
    QAction *quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void LoginWidget::initView()
{
    // 添加带有图标的选项
    QIcon icon1(":/resource/menuBar/emote3.png");
    QIcon icon2(":/resource/menuBar/emote3.png");
    QIcon icon3(":/resource/menuBar/emote3.png");

    ui->account_comboBox->addItem(icon1, "admin");
    ui->account_comboBox->addItem(icon2, "lht");
    ui->account_comboBox->addItem(icon3, "zxy");
    // 设置图标和文本之间的额外间隔
    for (int i = 0; i < ui->account_comboBox->count(); ++i) {
        QSize itemSize = ui->account_comboBox->sizeHint();
        itemSize.setWidth(itemSize.width() + 10); // 增加10像素的额外间隔
        ui->account_comboBox->setItemData(i, itemSize, Qt::SizeHintRole);
    }
    // 设置未展开状态的样式
    ui->account_comboBox->setStyleSheet(
                "QComboBox#account_comboBox { border: none; border-radius: 3px;padding: 9px;border-bottom: 2px solid black; }"
                "QComboBox#account_comboBox::drop-down { subcontrol-origin: padding; "
                "subcontrol-position: top right; width: 20px; border-left-width: 0px; "
                "border-left-color: gray; border-left-style: solid; }"
                "QComboBox#account_comboBox QAbstractItemView { border: none; }"
                "QComboBox#account_comboBox::down-arrow { image: url(:/resource/login/arrow.png); }"
                );
    setAttribute(Qt::WA_TranslucentBackground);
}

void LoginWidget::on_display_pwd_btn_clicked()
{
    if(ui->passwordEdit->echoMode() == QLineEdit::EchoMode::Normal){
        ui->passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
    }else{
        ui->passwordEdit->setEchoMode(QLineEdit::EchoMode::Normal);
    }

}

void LoginWidget::on_set_btn_clicked()
{
    auto conf  = GlobalConfig::getInstance()->getConnectConfig();
    QString ip = conf.ip;
    int port   = conf.port;
    m_loginSetDialog.init(ip,port);
    m_loginSetDialog.exec();
}

void LoginWidget::slotLoginStatus(int status, QString str)
{
    if(status){
        w.show();
        this->hide();
    }else{
        w.hide();
        this->show();
    }
}
