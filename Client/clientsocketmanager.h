// ClientSocketManager.h
#ifndef CLIENTSOCKETMANAGER_H
#define CLIENTSOCKETMANAGER_H

#include <QTcpSocket>

class ClientSocketManager : public QObject {
    Q_OBJECT
public:
    explicit ClientSocketManager(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void sendData(const QByteArray &data);

signals:
    void connected();
    void dataReceived(QByteArray data);

private slots:
    void handleConnected();
    void handleReadyRead();

private:
    QTcpSocket m_socket;
};

#endif // CLIENTSOCKETMANAGER_H
