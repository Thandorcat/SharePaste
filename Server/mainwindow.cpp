#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpServer.listen();
    QString Port = QString::number(tcpServer.serverPort());
    ui->address->setText(Port);
    numOfClients = 0;
    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));

}

void MainWindow::acceptConnection()
{
    QTcpSocket *connection = tcpServer.nextPendingConnection();
    soketList.append(connection);
    connect(connection, SIGNAL(readyRead()),
            this, SLOT(updateBuffer()));
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    ui->address->setText(QString::number(tcpServer.serverPort())+": " + QString::number(++numOfClients)
                        + " clients connected");
    qDebug()<<"Added new device";
}

void MainWindow::updateBuffer()
{
    QTcpSocket* socket;
    QListIterator<QTcpSocket*> socketIter(soketList);
    while(socketIter.hasNext())
    {
        socket= socketIter.next();
        if(socket->bytesAvailable()>0)
        {
            buffer = socket->readAll();
            QString temp(buffer);
            char type = temp.at(0).toLatin1();
            temp.remove(0,1);
            switch(type)
            {
                case 't':
                    temp.prepend("Text: ");
                    ui->recieve->setText(temp);
                    break;
                case 'f':
                    temp.prepend("File: ");
                    ui->recieve->setText(temp);
                    break;
                case'i':
                    temp.prepend("Image: ");
                    ui->recieve->setText(temp);
                    break;
                case 'u':
                    temp.prepend("Unknown: ");
                    ui->recieve->setText(temp);
                    break;
            }
            qDebug()<<buffer;
            NotifyAll(socket);
            break;
        }
    }

}

void MainWindow::NotifyAll(QTcpSocket* exept)
{
    QTcpSocket* socket;
    QListIterator<QTcpSocket*> socketIter(soketList);
    while(socketIter.hasNext())
    {
        socket= socketIter.next();
        if(socket!=exept)
        {
           socket->write(buffer);
        }
    }

}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    ui->recieve->setText("Error!");

    tcpServer.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
