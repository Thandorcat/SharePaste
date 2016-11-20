#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::SetClipboardField(QString newText)
{
    ui->clipboardField->setText(newText);
}

MainWindow::~MainWindow()
{
    delete ui;
}
