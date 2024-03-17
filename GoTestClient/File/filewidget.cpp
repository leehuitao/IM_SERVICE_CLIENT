#include "filewidget.h"
#include "ui_filewidget.h"
#include "fileitem.h"
FileWidget::FileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileWidget)
{
    ui->setupUi(this);
}

FileWidget::~FileWidget()
{
    delete ui;
}

void FileWidget::init()
{

}

void FileWidget::setProgress(int totalsize,int currentsize,int sendstatus,QString fileName)
{
    auto itemList = this->findChildren<FileItem *>();
    for (auto item : itemList)
    {
        if(item->getFileName() == fileName)
        {
            item->setProgess(totalsize,currentsize);
            //接收文件完成
            if(totalsize == currentsize)
            {
                m_fileList.removeOne(fileName);
                item->setParent(nullptr);
                if(itemList.size() == 1)
                    this->hide();
                delete item;
            }
        }
    }
}

void FileWidget::closeFile(QString md5)
{
    auto itemList = this->findChildren<FileItem *>();
    for (auto item : itemList)
    {
        if(item->getFileMd5() == md5)
        {
            m_fileList.removeOne(item->getFileName());
            item->setParent(nullptr);
            if(itemList.size() == 1)
                this->hide();
            delete item;

        }
    }
}

void FileWidget::setFileList(QString file)
{
    m_fileList.append(file);
}

void FileWidget::draw()
{
    auto itemList = this->findChildren<FileItem *>();
    //已经在界面上的文件
    QStringList strlist;
    for(auto it:itemList)
    {
        strlist.append(it->getFileName());
    }
    for(auto file:m_fileList)
    {
        if(strlist.contains(file))
            continue;
        FileItem * item = new FileItem;
        item->setFileName(file);
        int count = ui->verticalLayout->count() - 2;
        ui->verticalLayout->insertWidget(count,item);
        if(m_recvFile.contains(file))
            item->setFiletype(0);
        else
            item->setFiletype(1);
        connect(item,&FileItem::signFileCmd,[=](QString path,bool type,int cmd){
            signFileCmd(path,type,cmd);
            auto itemList = this->findChildren<FileItem *>();
            for (auto it : itemList){
                if(it == item){
                    m_fileList.removeOne(item->getFileName());
                    item->setParent(nullptr);
                    if(itemList.size() == 1)
                        this->hide();
                    delete item;
                }
            }
        });
    }
}

void FileWidget::addFileList(QString file,QString md5,bool sw)
{
    //    m_fileList.append(fileList);
    //    if(sw)
    //        m_recvFile.append(fileList);
    //    draw();
    FileItem * item = new FileItem;
    item->setFileName(file);
    item->setFileMd5(md5);
    int count = ui->verticalLayout->count() - 2;
    ui->verticalLayout->insertWidget(count,item);
    m_fileList.append(file);
    if(m_recvFile.contains(file))
        item->setFiletype(0);
    else
        item->setFiletype(1);
    connect(item,&FileItem::signFileCmd,this,&FileWidget::signFileCmd);
}

