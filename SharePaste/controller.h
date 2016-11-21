#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "clipboard.h"
#include "mainwindow.h"
#include "server.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    void SetMainwindow(MainWindow *w);
signals:

private slots:
    void UpdateClipboardField();
    void ServerBufferChanged(QString newText);
    void ConnectAttempt(QString adress, quint16 port);
private:
    Clipboard clipboard;
    MainWindow *window;
    Server server;
};

#endif // CONTROLLER_H
