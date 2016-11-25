#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <QObject>
#include <QImage>
#include <QVariant>
#include <server.h>
#include "clipboard.h"

class Synchronization : public QObject
{
    Q_OBJECT
public:
    explicit Synchronization(QObject *parent = 0);
    void SetServer(Server *serv);
    void SetData(const QMimeData *newData, QString type);
    void SendData();
signals:
    void DataSynchronized(QString);
public slots:
    void RecieveData(QByteArray revievedData);
private:
    Server *server;
    QString dataType;
    QByteArray byteData;
    Clipboard clipboard;
};

#endif // SYNCHRONIZATION_H
