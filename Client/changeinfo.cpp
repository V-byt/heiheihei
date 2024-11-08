#include "changeinfo.h"
#include "ui_changeinfo.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QDebug>
changeInfo::changeInfo(QWidget *parent,userInfo* in,ClientSocketManager* soc)
    : QMainWindow(parent)
    , ui(new Ui::changeInfo)
{
    ui->setupUi(this);
    this->setStyleSheet(styleSheet_btn);
    this->setWindowTitle("修改信息");
    this->info = in;
    this->socket = soc;
    ui->old_pass_edit->setPlaceholderText("请输入旧密码");
    ui->new_pass_edit->setPlaceholderText("请输入新密码(6-16个字符)");
    ui->new_pass_edit2->setPlaceholderText("请重复新密码");
    ui->photo_edit->setPlaceholderText("请输入新的联系方式(不修改就不输入)");
}

changeInfo::~changeInfo()
{
    delete ui;
}
//检验电话号码
bool isValidChineseMobilePhoneNumber(const QString &phoneNumber) {
    QRegularExpression phoneRegex("^1([358][0-9]|4[579]|66|7[0135678]|9[89])[0-9]{8}$");
    QRegularExpressionMatch match = phoneRegex.match(phoneNumber);
    return match.hasMatch();
}
void changeInfo::on_sure_btn_clicked()
{
    QString old_pass = ui->old_pass_edit->text();
    QString new_pass = ui->new_pass_edit->text();
    QString new_pass2 = ui->new_pass_edit2->text();
    QString photoNum = ui->photo_edit->text();
    if(old_pass != this->info->passWord){
        QMessageBox::warning(this,"警告","你的原密码不正确!请重试!");
        return;
    }
    if(new_pass != new_pass2){
        QMessageBox::warning(this,"警告","你两次输入的密码不相同,请重试!");
        return;
    }
    if(photoNum != "" && !isValidChineseMobilePhoneNumber(photoNum)){
        QMessageBox::warning(this,"警告","你输入的联系方式有误,请输入中国大陆手机号!");
        return;
    }
    this->info->passWord = new_pass;
    if(photoNum != ""){
        this->info->photoNum = photoNum;
    }
    QMessageBox::information(this,"提示","修改成功!");
    //给服务器发送修改信息的消息
    QString str = "changeInformation,";
    str.append(this->info->serialize());
    socket->sendData(str.toUtf8());
    this->close();
}


void changeInfo::on_exit_btn_clicked()
{
    this->close();
}

