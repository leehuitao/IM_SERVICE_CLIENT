#ifndef FILEITEM_H
#define FILEITEM_H

#include <QPushButton>
#include <QWidget>
#include <QFileDialog>
#include  <QDebug>
namespace Ui {
class FileItem;
}

class FileItem : public QWidget
{
    Q_OBJECT

public:
    explicit FileItem(QWidget *parent = nullptr);
    ~FileItem();

    void setFiletype(bool sw);

    void setFileMd5(QString md5){
        m_fileMd5 = md5;
    }

    void setFileName(QString name);

    void setProgess(int ,int);

    QString getFileMd5(){
        return m_fileMd5;
    }
    QString getFileName()
    {
        return path.split("/").last();
    }
public slots:

signals:
    /*
     * path包含路径和文件名
     * type 开始发送1，开始接收0
     * cmd  发送1，接收0，取消2
    */
    void signFileCmd(QString path,bool type,int cmd);


private slots:
    void on_sendOrRecv_btn_clicked();

    void on_cancel_btn_clicked();

    void on_pushButton_clicked();

private:
    Ui::FileItem *ui;
    QPushButton * btn ;
    QString saveFilePath;
    bool m_fileType = false;
    //fileName包含路径和文件名
    QString path;
    QString m_fileMd5;
};

#endif // FILEITEM_H
