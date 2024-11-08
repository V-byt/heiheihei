#include "shop.h"
#include "ui_shop.h"
#include <QMessageBox>
shop::shop(QWidget *parent,ClientSocketManager* so)
    : QMainWindow(parent)
    , ui(new Ui::shop)
{
    ui->setupUi(this);
    this->socket = so;
    this->setWindowTitle("商城");
    this->setStyleSheet(styleSheet_btn);
    this->setWindowIcon(QIcon("://images//shop.png"));
}

shop::~shop()
{
    delete ui;
}

void shop::on_pushButton_clicked()
{
    int n1 = ui->lineEdit->text().toInt();
    int n2 = ui->lineEdit_2->text().toInt();
    int n3 = ui->lineEdit_3->text().toInt();
    int n4 = ui->lineEdit_4->text().toInt();
    int n5 = ui->lineEdit_5->text().toInt();
    int n6 = ui->lineEdit_6->text().toInt();
    if((n1 >= 0) || (n2 >= 0) || (n3 >= 0) || (n4 >= 0) || n5 >= 0 || n6 >=0){
        QMessageBox::information(this,"提示","您的订单已经告知网管,请到餐后支付!");
        QString meg = "shop";
        meg.append(QString(",")).append(QString::number(n1));
        meg.append(QString(",")).append(QString::number(n2));
        meg.append(QString(",")).append(QString::number(n3));
        meg.append(QString(",")).append(QString::number(n4));
        meg.append(QString(",")).append(QString::number(n5));
        meg.append(QString(",")).append(QString::number(n6));
        socket->sendData(meg.toUtf8());
        this->close();
    }else{
        QMessageBox::warning(this,"警告","您输入了非法数据!");
    }
}

