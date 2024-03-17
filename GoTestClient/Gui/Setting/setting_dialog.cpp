#include "setting_dialog.h"
#include "ui_setting_dialog.h"
#include "Logic/app_cache.h"
#include "Logic/global_center.h"
#include <QFileDialog>
#include <QFile>
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::init(LoginBody body)
{
    ui->id_label->setText(QString::number(body.UserId));
    auto path = AppCache::Instance()->m_headImagePath[body.UserId];
    ui->head_label->setPixmap(QPixmap(path.isEmpty() ?":/resource/other.png":path));
    ui->name_label->setText(body.UserName);
    ui->phone_label->setText(body.PhoneNumber);
    ui->email_label->setText(body.Email);
}

void SettingDialog::updateHeadImage(QString path)
{
    ui->head_label->setPixmap(QPixmap(path));
}

void SettingDialog::on_change_head_clicked()
{
    // 创建一个文件对话框
        QFileDialog fileDialog;

        // 设置文件对话框的标题
        fileDialog.setWindowTitle("选择图片文件");

        // 设置文件对话框的工作模式为选择文件
        fileDialog.setFileMode(QFileDialog::ExistingFiles);

        // 设置文件过滤器，只允许选择图片文件
        fileDialog.setNameFilter("图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)");

        // 打开文件对话框并等待用户选择文件
        if (fileDialog.exec())
        {
            // 用户选择了文件
            QStringList selectedFiles = fileDialog.selectedFiles();

            if(selectedFiles.size()>0){
                auto filep = selectedFiles.at(0);
                ui->head_label->setPixmap(QPixmap(filep));
                //开始上传头像  生成第一个包
                FileBody body;
                QFile file;
                file.setFileName(filep);
                int size = file.size();
                body.UserId = AppCache::Instance()->m_userId;
                body.FileName = filep.split("/").last();
                body.SendUserId = AppCache::Instance()->m_userId;
                body.RecvUserId = 0;
                body.FileMD5 = createFileMd5(filep);
                body.TotalSize = size;
                body.CurrentSize = 0;
                body.FileSeek = 0;

                //启动发送文件第一个包
                Pack pack(body,UploadUserHeadImage,0);
                AppCache::Instance()->m_userHeadImagePath = filep;
                GlobalCenter::getInstance()->signUploadUserHeadImage(body);

            }
        }
}
