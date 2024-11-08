#ifndef CLIENTLOG_H
#define CLIENTLOG_H

#include <QMainWindow>
#include <QByteArray>
#include "clientsocketmanager.h"
namespace Ui {
class clientLog;
}

class clientLog : public QMainWindow
{
    Q_OBJECT

public:
    explicit clientLog(QWidget *parent = nullptr,ClientSocketManager *cskm = nullptr);
    ~clientLog();
public slots:
    void exit();
    void logIn();
    void changeEye();
    void handleData(QByteArray data);
private:
    Ui::clientLog *ui;
    ClientSocketManager *cskm;
    bool canSee = false;
    int num;

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
QMainWindow { background-image: url(://images//back1.jpg); }
)";
};

#endif // CLIENTLOG_H
