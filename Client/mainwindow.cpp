#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientsocketmanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QString>
#include "shop.h"
#include "changeinfo.h"
#include <QCloseEvent>
MainWindow::MainWindow(QWidget *parent,ClientSocketManager*cs,QDateTime*time,int comNum)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   //初始化前端界面
    ui->setupUi(this);
    this->setStyleSheet(styleSheet_btn);
    this->comNum = comNum;
    this->csktm = cs;
    this->timer = new QTimer();
    //向服务器索要数据
    csktm->sendData(QString("imformation").toUtf8());
    connect(csktm,&ClientSocketManager::dataReceived,this,&MainWindow::handleData);
    //显示上机时间
    this->startTime = time;
    ui->startTime_edit->setText(this->startTime->toString("yyyy-MM-dd hh:mm:ss"));
    this->setWindowTitle("客户端游玩界面");
    this->setWindowIcon(QIcon("://images//game.png"));
}
//每1s(2s)更新数据
void MainWindow::updataTime(){
    this->time_count++;
    // 更新现在时间
    int hours = time_count / 3600;
    int minutes = (time_count % 3600) / 60;
    int seconds = time_count % 60;
    QString str_time_count = QString("%1:%2:%3").arg(hours, 2, 10, QChar('0'))
                                 .arg(minutes, 2, 10, QChar('0'))
                                 .arg(seconds, 2, 10, QChar('0'));
    ui->keepTime_edit->setText(str_time_count);
    int addMoney = this->nowInfo.accountType == "VIP" ? 1 : 2;
    if(this->nowInfo.accountType == "VIP"){
        ui->tip_lab->setText("尊贵的VIP用户,您享受五折优惠!");
    }else{
        ui->tip_lab->setText("您还不是VIP,推荐您办理,可享五折优惠!");
    }
    this->consume_now += addMoney;
    ui->consume_edit->setText(QString::number(consume_now));
    if(this->isFirst && this->nowInfo.money - consume_now < 10){
        QMessageBox::warning(this,"警告","您剩余金额已经不足您使用10s,请及时充值!");
        this->isFirst = false;
    }
    if(this->nowInfo.money - consume_now < 0){
        QMessageBox::warning(this,"警告","您剩余金额已不足本次消费,下面将为您自动下机!");
        on_end_btn_clicked();
    }
}
//接收到用户数据后
void MainWindow::handle_information(QStringList list){
    if(list.size() != 6){
        qDebug() << "Error: information is not right";
        return;
    }
    userInfo temp(list[1],list[2],list[3].toInt(),list[4],list[5]);
    this->nowInfo = temp;
    //将Edit设置成只读
    ui->startTime_edit->setReadOnly(true);
    ui->keepTime_edit->setReadOnly(true);
    ui->comNum_edit->setReadOnly(true);
    ui->consume_edit->setReadOnly(true);
    ui->money_edit->setReadOnly(true);
    // 计算小时、分钟和秒
    int hours = time_count / 3600;
    int minutes = (time_count % 3600) / 60;
    int seconds = time_count % 60;
    QString str_time_count = QString("%1:%2:%3").arg(hours, 2, 10, QChar('0'))
                                 .arg(minutes, 2, 10, QChar('0'))
                                 .arg(seconds, 2, 10, QChar('0'));
    ui->keepTime_edit->setText(str_time_count);
    ui->consume_edit->setText(QString::number(this->consume_now));
    ui->money_edit->setText(QString::number(this->nowInfo.money));
    ui->comNum_edit->setText(QString::number(this->comNum));
    //qDebug() << this->nowInfo.accountType;
    if(this->nowInfo.accountType == "VIP"){
        ui->tip_lab->setText("尊贵的VIP用户,您享受五折优惠!");
    }else{
        ui->tip_lab->setText("您还不是VIP,推荐您办理,可享五折优惠!");
    }
    this->timer->start(1000);
    //每个按钮都已经在UI文件里connect了
    connect(this->timer,&QTimer::timeout,this,&MainWindow::updataTime);

}
//处理所有信息的函数
void MainWindow::handleData(QByteArray data){

    //qDebug() << data;
    QStringList list = QString::fromUtf8(data).split(',');
    if(list[0] == "information"){
        //接收用户的信息并处理
        handle_information(list);
    }else if(list[0] == "exitSuccess"){
        //用户下机请求通过
        this->csktm->disconnect();
        this->timer->stop();
        this->canDown = true;
        QString meg = QString("下机成功！您本次消费%1元，账户余额为%2元。欢迎您下次光临！")
                          .arg(QString::number(this->consume_now))
                          .arg(QString::number(this->nowInfo.money));
        QMessageBox::information(this,"提示",meg);

        this->close();
        QCoreApplication::exit();
    }else if(list[0] == "userTopup"){
        //用户充值后处理
        handle_top_up(list);
    }else if(list[0] == "VIP"){
        //用户VIP办理成功
        this->nowInfo.accountType = "VIP";
        QMessageBox::information(this,"通知","恭喜您成为了我们尊贵的VIP,接下来的消费您将享受五折优惠!");
    }else if(list[0] == "logOut"){
        //用户注销成功
        QMessageBox::information(this,"通知","您的账户已经注销,即刻将为您下机");
        this->canDown = true;
        this->csktm->disconnect();
        this->close();
        QCoreApplication::exit();
    }
}
//用户充值后处理
void MainWindow::handle_top_up(QStringList data){
    this->nowInfo.money = data[1].toInt();
    this->ui->money_edit->setText(data[1]);
}
//重写关闭函数(防止逃单)
void MainWindow::closeEvent(QCloseEvent* en){
    if(canDown){
        QMainWindow::closeEvent(en);
    }else{
        QMessageBox::warning(this,"警告","请通过合法手段下机");
        en->ignore();
    }
}
//析构函数
MainWindow::~MainWindow()
{
    delete ui;
}
//结束上网
void MainWindow::on_end_btn_clicked()
{
    //发送exit信息exit,dataInfo,机号
    QString data = "exit,";
    this->nowInfo.money -= consume_now;
    data.append(this->nowInfo.serialize()).append(',').append(QString::number(this->comNum));
    this->csktm->sendData(data.toUtf8());
}

//信息修改
void MainWindow::on_changeInfo_btn_clicked()
{
    changeInfo * w = new changeInfo(this,&(this->nowInfo),csktm);
    w->show();
}

//呼叫网管按钮
void MainWindow::on_call_btn_clicked()
{
    QString meg = "call,";
    meg.append(QString::number(this->comNum));
    this->csktm->sendData(meg.toUtf8());
    QMessageBox::information(this,"提示","已为您呼叫网管，请耐心等候");
}

//会员办理
void MainWindow::on_getVIP_btn_clicked()
{
    if(this->nowInfo.accountType == "VIP"){
        QMessageBox::information(this,"通知","您已经是尊贵的VIP了,不需重复办理");
        return;
    }
    QString meg = "getVIP,";
    meg.append(QString::number(this->comNum));
    this->csktm->sendData(meg.toUtf8());
    QMessageBox::information(this,"通知","您的请求已经通知网管了,请静待回复!");
}

//账户注销
void MainWindow::on_logout_btn_clicked()
{
    // 创建一个QMessageBox对象，用于显示询问信息
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "确认操作", // 对话框标题
                                  "你确定要注销账户吗?注销后账户余额不予退还且会即刻下机!", // 对话框中显示的问题
                                  QMessageBox::Yes|QMessageBox::No); // 提供的按钮，Yes和No

    if (reply == QMessageBox::Yes) {
        QString meg = "logOut,";
        meg.append(QString::number(this->comNum));
        this->csktm->sendData(meg.toUtf8());
    }
}

//在线点餐
void MainWindow::on_order_btn_clicked()
{
    shop* w = new shop(this,this->csktm);
    w->show();
}

