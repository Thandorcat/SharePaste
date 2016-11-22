#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <QObject>
#include <QImage>
#include <QVariant>
#include <server.h>

class Synchronization : public QObject
{
    Q_OBJECT
public:
    explicit Synchronization(QObject *parent = 0);
    void SetServer(Server *serv);
    void SetData(const QMimeData *newData, QString type);
    void SendData();
signals:
    void DataSynchronized(QMimeData*, QString);
public slots:
    void RecieveData(QByteArray revievedData);
private:
    Server *server;
    QString dataType;
    const QMimeData *data;
    QMimeData *recievedData;
    QByteArray byteData;
};

#endif // SYNCHRONIZATION_H
