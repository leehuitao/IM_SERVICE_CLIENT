#include "fileitem.h"
#include "ui_fileitem.h"
#pragma execution_character_set("utf-8")

FileItem::FileItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileItem)
{
    ui->setupUi(this);
    ui->pushButton->hide();
}

FileItem::~FileItem()
{
    delete ui;
}
//true 为发送方   fasle为接受方
void FileItem::setFiletype(bool sw)
{
    m_fileType = sw;
    if(sw)
    {
        ui->sendOrRecv_btn->setText(QString("发送"));
//        ui->label->setText(QString("发"));
    }
    else
    {
        ui->sendOrRecv_btn->setText(QString("接收"));
        ui->pushButton->show();
//        ui->label->setText(QString("收"));
    }

}

void FileItem::setFileName(QString name)
{
    path = name;
    QString name2 = name.split("/").last();
    ui->fileName->setText(name2);
    QString suffix = name2.split(".").last();//后缀名
    if(suffix!="jpg"&&suffix!="png"&&suffix!="txt")
        suffix = "file";
    QString pngName = QString(":/resource/file/%1.png").arg(suffix);
    QPixmap pixmap(pngName);
    ui->avatar_lab->setPixmap(pixmap);
    ui->avatar_lab->setScaledContents(true); // 设置以缩放内容以填充QLabel
    ui->avatar_lab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 设置QLabel的大小策略
}

void FileItem::setProgess(int totalsize, int currentsize)
{
    int progress = double(currentsize)/double(totalsize)*100;
    ui->progressBar->setValue(progress);
}

void FileItem::on_sendOrRecv_btn_clicked()
{
    if(m_fileType)
    {
        signFileCmd(path,m_fileType,m_fileType);
    }
    //接收文件
    else
    {
        signFileCmd("C:/data/"+path.split("/").last(),m_fileType,m_fileType);
    }
}
//包含取消发送和取消接收
void FileItem::on_cancel_btn_clicked()
{
    signFileCmd(m_fileMd5,m_fileType,2);
}

//另存为
void FileItem::on_pushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(
                this,
                "Select Folder",
                QDir::homePath());

    if (!folderPath.isEmpty()) {
        QString name2 = path.split("/").last();
        path = folderPath + "/" + name2;
        qDebug()<<""<<path;
        signFileCmd(path,m_fileType,m_fileType);
    }
}
