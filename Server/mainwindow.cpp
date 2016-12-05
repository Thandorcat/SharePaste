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
    connect(connection, SIGNAL(disconnected()),
            this, SLOT(Disconnected()));
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    ui->address->setText(QString::number(tcpServer.serverPort())+": " + QString::number(++numOfClients)
                        + " clients connected");
    qDebug()<<"Added new device";
}

void MainWindow::updateBuffer()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    buffer = socket->readAll();
    buffer.size();
    char type = buffer.at(0);
    switch(type)
    {
        case 't':
        {
            QString temp(buffer);
            temp.remove(0,1);
            temp.prepend("Text: ");
            ui->recieve->setText(temp);
            break;
        }
        case 'f':
        {
            ui->recieve->setText("File");
            break;
        }
        case'i':
        {
            ui->recieve->setText("Bitmap image");
            break;
        }
        case 'u':
        {
            QString temp(buffer);
            temp.remove(0,1);
            temp.prepend("Unknown: ");
            ui->recieve->setText(temp);
            break;
        }
    }
    NotifyAll(socket);

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
           qDebug()<<socket->write(buffer);
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

void MainWindow::Disconnected()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    soketList.removeOne(socket);
    if(--numOfClients>0)
        ui->address->setText(QString::number(tcpServer.serverPort())+": " + QString::number(numOfClients)
                              + " clients connected");
    else
        ui->address->setText(QString::number(tcpServer.serverPort()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
