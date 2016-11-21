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
    QString adress = ui->adressField->toPlainText();
    quint16  port = ui->portField->toPlainText().toUInt();
    qDebug()<<endl<<"Connect pressed: "<<adress<<":"<<port;
    emit ConnectRequest(adress, port);
}

MainWindow::~MainWindow()
{
    delete ui;
}
