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
    ui->adress->setText(Port);
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
            QByteArray recieved = socket->readAll();
            buffer = QString(recieved.data());
            ui->recieve->setText(buffer);
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
           socket->write(buffer.toUtf8());
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
