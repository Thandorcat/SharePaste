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
            buffer = socket->readAll();
           /* QByteArray temp(&buffer);
            char type = temp.at(0);
            temp.remove(0,1);
            switch(type)
            {
                case 't':
                    recievedData->setData("text/plain", revievedData);
                    dataType = QString("Text");
                    emit DataSynchronized(recievedData, dataType);
                    break;
                case 'f':
                    recievedData->setData("text/plain", revievedData);
                    dataType = QString("File");
                    emit DataSynchronized(recievedData, dataType);
                    break;
                case'i':
                    recievedData->setData("application/x-qt-image", revievedData);
                    dataType = QString("Image");
                    emit DataSynchronized(recievedData, dataType);
                    break;
                case 'u':
                    recievedData->setData("text/plain", revievedData);
                    dataType = QString("Unknown");
                    emit DataSynchronized(recievedData, dataType);
                    break;
            }*/
            qDebug()<<buffer;
            ui->recieve->setText(QString(buffer));
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
