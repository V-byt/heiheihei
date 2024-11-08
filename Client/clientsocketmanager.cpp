// ClientSocketManager.cpp
#include "ClientSocketManager.h"

ClientSocketManager::ClientSocketManager(QObject *parent) : QObject(parent) {
    connect(&m_socket, &QTcpSocket::connected, this, &ClientSocketManager::handleConnected);
    connect(&m_socket, &QTcpSocket::readyRead, this, &ClientSocketManager::handleReadyRead);
}

void ClientSocketManager::connectToServer(const QString &host, quint16 port) {
    m_socket.connectToHost(host, port);
}

void ClientSocketManager::sendData(const QByteArray &data) {
    m_socket.write(data);
}

void ClientSocketManager::handleConnected() {
    emit connected();
}

void ClientSocketManager::handleReadyRead() {
    QByteArray data = m_socket.readAll();
    emit dataReceived(data);
}
