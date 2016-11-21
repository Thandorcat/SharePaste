#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    connected = false;
}

bool Server::connectToServer(QString adress, quint16 port)
{
    socket.connectToHost(adress, port);
    QThread::msleep(1000);
    qDebug()<<socket.state();
    if(socket.state()==3)
        return true;
    else
        return false;
}

void Server::sendToServer(QString text)
{
    socket.write(text.toUtf8());
}
