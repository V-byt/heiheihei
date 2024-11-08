#ifndef SHOP_H
#define SHOP_H

#include <QMainWindow>
#include "clientsocketmanager.h"
namespace Ui {
class shop;
}

class shop : public QMainWindow
{
    Q_OBJECT

public:
    explicit shop(QWidget *parent = nullptr,ClientSocketManager* socket = nullptr);
    ~shop();

private slots:
    void on_pushButton_clicked();

private:
    Ui::shop *ui;
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

#endif // SHOP_H
