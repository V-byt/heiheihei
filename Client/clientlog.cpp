#include "clientlog.h"
#include "ui_clientlog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
clientLog::clientLog(QWidget *parent,ClientSocketManager *csk)
    : QMainWindow(parent)
    , ui(new Ui::clientLog)
{
    ui->setupUi(this);
    this->setStyleSheet(styleSheet_btn);
    this->cskm = csk;
    this->setWindowTitle("用户登录界面");
    this->setWindowIcon(QIcon("://images//user.png"));
    ui->name_edit->setPlaceholderText("请输入您的身份证号");
    ui->pass_edit->setPlaceholderText("请输入您的密码");
    ui->num_edit->setPlaceholderText("输入机器号(1-300整数)");
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->eye->setIcon(QIcon("://images//eye_false.png"));
    ui->eye->setIconSize(QSize(50,40));
    ui->eye->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    connect(ui->exit_btn,&QPushButton::clicked,this,&clientLog::exit);
    connect(ui->log_btn,&QPushButton::clicked,this,&clientLog::logIn);
    connect(ui->eye,&QPushButton::clicked,this,&clientLog::changeEye);
    connect(cskm,&ClientSocketManager::dataReceived,this,&clientLog::handleData);
}
void clientLog::exit(){
    this->close();
}
//处理接收到的信息
void clientLog::handleData(QByteArray data){
    QString str_data = QString::fromUtf8(data);
    if(str_data == "success"){
        disconnect(cskm,&ClientSocketManager::dataReceived,this,&clientLog::handleData);
        QMessageBox::information(this,"提示","您已经登录成功了,开始爽玩吧!");
        this->close();
        QDateTime cur = QDateTime::currentDateTime();
        MainWindow * w = new MainWindow(this,this->cskm,&cur,num);
        w->show();
    }else if(str_data == "toUp"){
        QMessageBox::warning(this,"警告","您还没有到网管处进行上机授权,请授权后重试!");
    }else if(str_data == "passWord"){
        QMessageBox::warning(this,"警告","您输入的密码错误,请重试!");
    }else if(str_data == "notRegister"){
        QMessageBox::warning(this,"警告","您还没有注册,请到网管处注册!");
    }else if(str_data == "hasPeople"){
        QMessageBox::warning(this,"警告","这个机位已经有人了,请换一个机器号!");
    }
}
//登录按钮点击后
void clientLog::logIn(){
    QString name = ui->name_edit->text();
    QString passWord = ui->pass_edit->text();
    int num = ui->num_edit->text().toInt();
    if(num <= 0 || num > 300){
        QMessageBox::warning(this,"警告","非法机器号!请重试");
        return;
    }
    if(name == "" || passWord == ""){
        QMessageBox::warning(this,"警告","用户名和密码不能为空!");
        return;
    }
    QString meg = QString("0,") + name + QString(",") + passWord + QString(",") + QString::number(num);
    this->num = num;
    cskm->sendData(meg.toUtf8());
}

//眼睛按钮点击后
void clientLog::changeEye(){
    if(this->canSee){
        canSee = false;
        ui->pass_edit->setEchoMode(QLineEdit::Password);
        ui->eye->setIcon(QIcon("://images//eye_false.png"));
    }else{
        canSee = true;
        ui->eye->setIcon(QIcon("://images//eye_true.png"));
        ui->pass_edit->setEchoMode(QLineEdit::Normal);
    }
    this->repaint();
}
clientLog::~clientLog()
{
    delete ui;
}
