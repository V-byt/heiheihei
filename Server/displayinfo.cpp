#include "displayinfo.h"
#include "ui_displayinfo.h"

displayInfo::displayInfo(QWidget *parent,userInfo* data)
    : QMainWindow(parent)
    , ui(new Ui::displayInfo)
{
    this->data = data;
    ui->setupUi(this);
    this->setWindowIcon(QIcon("://images//user.png"));
    this->setWindowTitle("用户信息展示");
    ui->lineEdit->setText(data->name);
    ui->lineEdit_2->setText(data->photoNum);
    ui->lineEdit_3->setText(data->accountType);
    ui->lineEdit_4->setText(data->passWord);
    ui->lineEdit_5->setText(QString::number(data->money));
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
    ui->lineEdit_4->setReadOnly(true);
    ui->lineEdit_5->setReadOnly(true);
}

displayInfo::~displayInfo()
{
    delete ui;
}
