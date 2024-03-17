#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Network/LhtTcpClient/lht_tcp_client.h"
#include "Gui/ChatUi/emotion_widget.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>
#include "Logic/app_cache.h"
#include "sql/sqlite.h"
#include "Gui/ChatUi//screenwidget.h"
#include "./File/filewidget.h"
#include "Gui/ChatUi/send_text_edit.h"
#include "Gui/ChatUi/msg_widget_item.h"
#include "Gui/ChatUi/video_widget.h"
#include "Gui/ChatUi/voice_widget.h"
#include "Gui/ChatUi/invite_widget.h"
#include "Gui/system_set_dialog.h"
#include "mainwindow_new.h"
#include "Gui/Setting/setting_dialog.h"
#include "utils/voice/audio_play.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void getMsg(QString msgid);

public slots:

    void shakeWindow();

    void on_sendmsg_btn_clicked();

    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_emoji_btn_clicked();
    void on_status_comboBox_currentIndexChanged(int index);
    void on_screen_btn_clicked();
    void on_screen_noself_btn_clicked();
    void on_file_btn_clicked();
    void on_msg_btn_clicked();
    void on_org_btn_clicked();
    void on_header_btn_clicked();
    void on_group_btn_clicked();
    void on_set_btn_clicked();
    void on_msg_listWidget_itemClicked(QListWidgetItem *item);
    void on_test_btn_clicked();
    void on_jitter_btn_clicked();
    void on_audio_btn_clicked();
    void on_video_btn_clicked();

    virtual void resizeEvent(QResizeEvent *event);
signals:
    void signUpdataUnread(int id,int number);

    void signLogout();
protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器
    void mousePressEvent(QMouseEvent *ev);
private:

private:
    Ui::MainWindow *ui;
    QString                         m_userName;
    EmotionWidget *                 m_emojiWidget = nullptr;
    VoiceWidget*                    m_voiceWidget = nullptr;
    VideoWidget*                    m_videoWidget = nullptr;
    InviteWidget*                   m_inviteWidget = nullptr;
    QMap<QString,QStandardItem*>    m_userName2UiPointer;

    SettingDialog                   m_setDialog;
//    FileWidget      *m_fileWidget = nullptr;
    QMap<int,FileWidget*>           m_fileWidgets;
    QString                         m_fileSendName;
//    MainWindowNew   mainWindowNew;
private slots:


};
#endif // MAINWINDOW_H
