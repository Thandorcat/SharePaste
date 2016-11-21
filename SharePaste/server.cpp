#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    connected = false;
}

bool Server::connectToServer(QString adress, quint16 port)
{
    socket.connectToHost(adress, port);
    QThread::msleep(1000);
    connect(&socket, SIGNAL(readyRead()),
            this, SLOT(updateFromServer()));
    if(socket.state()==3)
        return true;
    else
        return false;
}

void Server::sendToServer(QString text)
{
    socket.write(text.toUtf8());
}

void Server::updateFromServer()
{
    QByteArray recieved = socket.readAll();
    QString text = QString(recieved.data());
    emit  BufferChanged(text);
}

