#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QDebug>
#include <QTcpSocket>
#include <QtNetwork>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    bool ConnectToServer(QString adress, quint16 port);
    void SendToServer(QByteArray data);
signals:
    void BufferChanged(QByteArray newBuffer);
public slots:
    void updateFromServer();
private:
    QTcpSocket socket;
    QString adress;
    quint16 port;
    bool connected;
};

#endif // SERVER_H
