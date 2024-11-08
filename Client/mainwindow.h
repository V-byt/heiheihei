#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "userinfo.h"
#include <QMainWindow>
#include <QDateTime>
#include <QTime>
#include "clientsocketmanager.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,ClientSocketManager *csktm = nullptr,QDateTime *startTime = nullptr,int num = 0);
    ~MainWindow();
    //接收用户数据的处理函数
    void handle_information(QStringList li);
    //接收用户充值后金额
    void handle_top_up(QStringList list);
public slots:
    void handleData(QByteArray data);
    void updataTime();
private slots:
    void on_end_btn_clicked();
    void on_changeInfo_btn_clicked();

    void on_call_btn_clicked();

    void on_getVIP_btn_clicked();

    void on_logout_btn_clicked();

    void on_order_btn_clicked();

protected :
    void closeEvent(QCloseEvent* en);

private:
    Ui::MainWindow *ui;
    ClientSocketManager *csktm;
    QDateTime* startTime;
    //当前使用的用户信息
    userInfo nowInfo;
    //当前机号
    int comNum;
    //本次使用时间单位(s)
    int time_count = 0;
    //用户提示(VIP可以有5折)
    QString tip = "";
    //本次已经消耗的金额
    int consume_now = 0;
    QTimer* timer;
    //记录是否是合法下机
    bool canDown = false;
    //是否是第一次提醒
    bool isFirst = true;


    QString styleSheet_btn= R"(
QPushButton {
    background-color: #473C8B;
    border-style: outset;
    border-width: 2px;
    border-radius: 5px;
    border-color: #8B7355;
    font: bold 20px;
    min-width: 2em;
    color: white;
    font-family: '华文新魏';
    padding: 5px;
}

QPushButton:hover {
    background-color: rgb(0, 150, 0);
}

QPushButton:pressed {
    background-color: #1E90FF;
    border-style: inset;
}

QPushButton:!enabled {
    background-color: rgb(100, 100, 100);
    border-style: inset;
}
)";
};
#endif // MAINWINDOW_H
