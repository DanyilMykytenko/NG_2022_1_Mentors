#include "servak.h"

Servak::Servak(QString ip, int port, QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer();
    m_server->listen(QHostAddress(ip), port);

    if(m_server->isListening())
    {
        qDebug() << "Server started at: " <<
                    m_server->serverAddress() <<
                    m_server->serverPort();
    }
    else
        qDebug() << "Server failed to start " <<
                    m_server->serverError();
    connect(m_server, &QTcpServer::newConnection, this, &Servak::newConnectionFromClient);
}

void Servak::newConnectionFromClient()
{
    QTcpSocket *client = m_server->nextPendingConnection();
    m_clients.push_back(client);
    qDebug() << "New client from: " << client->localAddress();

    connect(client, &QTcpSocket::readyRead, this, &Servak::readyRead);
    connect(client, &QTcpSocket::disconnected, this, &Servak::disconnectedClient);
}

void Servak::disconnectedClient()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    m_clients.removeOne(client);

    qDebug() << "Disconnected client from: " <<
                client->localAddress();

    disconnect(client, &QTcpSocket::readyRead, this, &Servak::readyRead);
    disconnect(client, &QTcpSocket::disconnected, this, &Servak::disconnectedClient);
}

void Servak::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray data = client->readAll();

    if(data.indexOf("<n>") == 0)
    {
        QByteArray login = data.remove(0, 3);
        m_logins[client] = login;
        qDebug() << " as " + login;
    }
    if(data.indexOf("<m>") == 0)
    {
        QByteArray message = data.remove(0, 3);
        sendToAll(m_logins[client]  + ": " + message);
    }
}

void Servak::sendToAll(QByteArray message)
{
    for(QTcpSocket* buffer : m_clients)
    {
        buffer->write(message);
    }
}
