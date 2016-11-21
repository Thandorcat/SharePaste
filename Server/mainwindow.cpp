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
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    ui->adress->setText("Success");
    tcpServer.close();
}

void MainWindow::updateServerProgress()
{
    QByteArray recieved = tcpServerConnection->readAll();
    QString recieved_str = QString(recieved.data());
    ui->recieve->setText(recieved_str);

}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    ui->recieve->setText("Error: "+tcpServerConnection->errorString());

    tcpServer.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
