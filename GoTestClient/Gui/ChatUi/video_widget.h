#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QWidget>

namespace Ui {
class VideoWidget;
}

class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();

    void init();
    void setSignSw(bool sw){
        m_sendCloseSign =sw;
    }
    void setHeadUrl(const QPixmap &pixmap);
    void setNickNameInfo(const QString& nameInfo = {});
    void setStatusInfo(const QString& statusInfo = {});
    void setRemoteLabelHide(bool status);
    void setConnectStatus(bool status){m_connceted = status;}

    void paintRemoteVideo(const QImage &img);
    void paintLocalVideo(const QImage &img);
signals:
    void signCloseVideo();
protected slots:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_call_toolBtn_clicked();

private:
    Ui::VideoWidget *ui;

    bool m_sendCloseSign =1;
    bool m_connceted = false;
};

#endif // VIDEO_WIDGET_H
