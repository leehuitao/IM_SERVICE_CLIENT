
#ifndef VOICEWIDGET_H
#define VOICEWIDGET_H

#include <QWidget>
#include <QGraphicsBlurEffect>


QT_BEGIN_NAMESPACE
namespace Ui { class VoiceWidget; }
QT_END_NAMESPACE

class VoiceWidget : public QWidget

{
    Q_OBJECT

public:
    VoiceWidget(QWidget *parent = nullptr);
    ~VoiceWidget();

    void init();
    void setSignSw(bool sw){
        m_sendCloseSign =sw;
    }
    void setHeadUrl(const QPixmap &pixmap);
    void setNickNameInfo(const QString& nameInfo = {});
    void setStatusInfo(const QString& statusInfo = {});
signals:
    void signCloseVoice();
private slots:
    void on_call_toolBtn_clicked();
    void closeEvent(QCloseEvent* event);

private:
    Ui::VoiceWidget *ui;
    bool m_sendCloseSign =1;
};

#endif // VOICEWIDGET_H
