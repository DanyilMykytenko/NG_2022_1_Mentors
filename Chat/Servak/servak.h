#ifndef SERVAK_H
#define SERVAK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

class Servak : public QObject
{
    Q_OBJECT
public:
    explicit Servak(QString ip, int port, QObject *parent = nullptr);

private slots:
    void newConnectionFromClient();
    void disconnectedClient();
    void readyRead();

    void sendToAll(QByteArray message);

private:
    QTcpServer *m_server;
    QVector<QTcpSocket*> m_clients;
    QMap<QTcpSocket*, QByteArray> m_logins;
    // zero:0
    // m_logins[zero]
    // 0
};

#endif // SERVAK_H
