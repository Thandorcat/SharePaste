#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QDebug>
#include <QTcpSocket>
#include <QList>

#define MAX_SOKETS 20

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void acceptConnection();
    void updateBuffer();
    void displayError(QAbstractSocket::SocketError socketError);
private:
    Ui::MainWindow *ui;
    QTcpServer tcpServer;
    QList<QTcpSocket*> soketList;
    QByteArray buffer;
    void NotifyAll(QTcpSocket* exept);
};

#endif // MAINWINDOW_H
