#include "clientlog.h"
#include <QApplication>
#include <QFile>
#include "clientsocketmanager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientSocketManager *m = new ClientSocketManager();
    QFile file(":/ip.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
        return -1;
    }

    QTextStream in(&file);
    QString ip = in.readAll();


    file.close();

    m->connectToServer(ip,1314);
    clientLog w(nullptr,m);
    w.show();
    return a.exec();
}
