#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>

namespace Ui {
class FileWidget;
}

class FileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileWidget(QWidget *parent = nullptr);
    ~FileWidget();
    void init();
    void setFileList(QString file);
//    void addFileList(QStringList fileList);
    void addFileList(QString file ,QString md5, bool sw = false);
    void draw();
    void setProgress(int totalsize,int currentsize,int sendstatus,QString fileName);
    void closeFile(QString md5);
    //获取剩余文件数量
    int getRemainderFileCount()
    {
        return m_fileList.size();
    }
public slots:

signals:
    void signFileCmd(QString name,bool type,int cmd);

private:
    Ui::FileWidget *ui;
    //所有文件
    QStringList     m_fileList;
    //接收的文件
    QStringList     m_recvFile;
};

#endif // FILEWIDGET_H
