#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QHash>
#include <QSqlDatabase>
#include <QTcpServer>
#include "userinfo.h"
namespace Ui {
class mainWin;
}

class mainWin : public QMainWindow
{
    Q_OBJECT

public:
    //当充值完毕后,更新数据:
    void updata_top(QString top_id);
    //方便地更新(i,j)的数据
    void setTableValue(int i,int j,QVariant val);
    //保存信息
    void saveData();
    //登录信息(0) 处理
    void handleData0(QStringList data,QTcpSocket* so);
    //处理用户信息索要请求
    void sendInfoData(QStringList data,QTcpSocket* so);
    //处理用户下机请求
    void handleUserDown(QStringList data,QTcpSocket* so);
    //处理用户信息修改
    void handleUserInfoChange(QStringList data,QTcpSocket* so);
    //处理呼叫网管 call,机号,ID
    void whenCalled(QStringList data);
    //处理用户办理会员的请求
    void handleGetVIP(QStringList data,QTcpSocket* so);
    //处理用户注销账号请求
    void handleLogOut(QStringList data,QTcpSocket* so);
    explicit mainWin(QWidget *parent = nullptr);
    ~mainWin();
public slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();
    void on_reg_btn_clicked();

    void on_topUp_btn_clicked();

    void on_exit_btn_clicked();
    void on_board_btn_clicked();

    void on_control_btn_clicked();

protected:
    void closeEvent(QCloseEvent* e);
private slots:
    void on_search_btn_clicked();

    void on_query_btn_clicked();

private:
    //储存现在可以上机的用户ID
    QHash<QString,int> canBorad;
    //储存现在启动的机器号及其用户(假设有300个机器)
    QHash<QString,QString> computerToUser;
    QHash<QString,QString> userToComputer;
    //储存现在注册好的用户的信息
    QHash<QString,userInfo> userData;
    QTcpServer* m_server;

    QList<QTcpSocket*> m_clients;
    //ID和Socket互存;
    QHash<QString,QTcpSocket*> idToS;
    QHash<QTcpSocket*,QString> sToId;
    //储存机器号->行号
    std::vector<int> rows = std::vector<int>(301,-1);
    Ui::mainWin *ui;
    QSqlDatabase db;
    //存储呼叫网管机号
    QHash<QString,int> calles;
    QString styleSheet_btn= R"(
QPushButton {
    background-color: #473C8B;
    border-style: outset;
    border-width: 2px;
    border-radius: 5px;
    border-color: #8B7355;
    font: bold 18px;
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
QMainWindow { background-image: url(://images//back2.jpg); }
)";
};

#endif // MAINWIN_H
