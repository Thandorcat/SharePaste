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
    bool connectToServer(QString adress, quint16 port);
    void sendToServer(QString text);
signals:
    void BufferChanged(QString newBuffer);
public slots:
    void updateFromServer();
private:
    QTcpSocket socket;
    QString adress;
    quint16 port;
    bool connected;
};

#endif // SERVER_H
