#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "clipboard.h"
#include "mainwindow.h"
#include "server.h"
#include "synchronization.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    void SetMainwindow(MainWindow *w);
signals:

private slots:
    void UpdateClipboardField(QString dataType);
    void SyncClipboard();
    void ServerBufferSyncronized(QString);
    void ConnectAttempt(QString adress, quint16 port);
    void Disonnect();
private:
    Clipboard clipboard;
    MainWindow *window;
    Server server;
    Synchronization sync;
    bool writeFlag = false;
    bool connected = false;
};

#endif // CONTROLLER_H
