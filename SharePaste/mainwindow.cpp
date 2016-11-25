#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(ConnectPressed()));
}

void MainWindow::SetClipboardField(QString newText)
{
    ui->clipboardField->setText(newText);
}

void MainWindow::ConnectPressed()
{
    if(!connected)
    {
        QString adress = ui->adressField->toPlainText();
        quint16  port = ui->portField->toPlainText().toUInt();
        qDebug()<<endl<<"Connect pressed: "<<adress<<":"<<port;
        ui->connectButton->setText("Disconnect");
        connected = true;
        emit ConnectRequest(adress, port);
    }
    else
    {
        qDebug()<<endl<<"Disconnent!";
        ui->connectButton->setText("Connect");
        connected = false;
        emit Disconnect();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
