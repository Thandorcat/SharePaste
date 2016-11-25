#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    connected = false;
}

bool Server::ConnectToServer(QString adress, quint16 port)
{
    socket.connectToHost(adress, port);
    connect(&socket, SIGNAL(readyRead()),
            this, SLOT(updateFromServer()));
    if(socket.state()==3)
        return true;
    else
        return false;
}

void Server::DisconnectFromServer()
{
    socket.disconnectFromHost();
}

void Server::SendToServer(QByteArray data)
{
    socket.write(data);
}

void Server::updateFromServer()
{
    QByteArray recieved = socket.readAll();
    emit  BufferChanged(recieved);
}

