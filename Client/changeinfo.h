#ifndef CHANGEINFO_H
#define CHANGEINFO_H

#include <QMainWindow>
#include "userinfo.h"
#include "clientsocketmanager.h"
namespace Ui {
class changeInfo;
}

class changeInfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit changeInfo(QWidget *parent = nullptr,userInfo* in = nullptr,ClientSocketManager* socket = nullptr);
    ~changeInfo();

private slots:
    void on_sure_btn_clicked();

    void on_exit_btn_clicked();

private:
    Ui::changeInfo *ui;
    userInfo* info;
    ClientSocketManager* socket;


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

#endif // CHANGEINFO_H
