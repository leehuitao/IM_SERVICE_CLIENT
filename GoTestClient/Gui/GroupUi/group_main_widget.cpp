#include "group_main_widget.h"
#include "ui_group_main_widget.h"

GroupMainWidget::GroupMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupMainWidget)
{
    ui->setupUi(this);
}

GroupMainWidget::~GroupMainWidget()
{
    delete ui;
}

void GroupMainWidget::init()
{

}
