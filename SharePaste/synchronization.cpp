#include "synchronization.h"

Synchronization::Synchronization(QObject *parent) : QObject(parent)
{
    recievedData = new QMimeData();
}

void Synchronization::SetServer(Server *serv)
{
    server = serv;
    connect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveData(QByteArray)));
}

void Synchronization::SetData(const QMimeData *newData, QString type)
{
    data = newData;
    dataType = type;
}

void Synchronization::SendData()
{
    QStringList types;
    types << "Text" << "File" << "Image"<<"Unknown";
    switch(types.indexOf(dataType))
    {
        case 0:
            byteData = data->data("text/plain");
            byteData.prepend('t');
            server->SendToServer(byteData);
            break;
        case 1:
            byteData = data->data("text/plain");
            byteData.prepend('f');
            server->SendToServer(byteData);
            break;
        case 2:
            qDebug()<<data->hasImage();
            byteData = data->imageData().toByteArray();
            qDebug()<<byteData.size();
            byteData.prepend('i');
            server->SendToServer(byteData);
            break;
        case 3:
            byteData = data->data("text/plain");
            byteData.prepend('u');
            server->SendToServer(byteData);
            break;
    }
}

void Synchronization::RecieveData(QByteArray revieved)
{
    qDebug()<<"Recieved data: "<<revieved;
    char type = revieved.at(0);
    revieved.remove(0,1);
    switch(type)
    {
        case 't':
        {
            qDebug()<<"Text";
            recievedData->setText(QString(revieved));
            dataType = QString("Text");
            emit DataSynchronized(recievedData, dataType);
            break;
        }
        case 'f':
        {
            recievedData->setData("text/plain", revieved);
            dataType = QString("File");
            emit DataSynchronized(recievedData, dataType);
            break;
        }
        case'i':
        {
            QVariant variant(revieved);
            QImage image= variant.value<QImage>();
            recievedData->setImageData(image);
            dataType = QString("Image");
            emit DataSynchronized(recievedData, dataType);
            break;
        }
        case 'u':
        {
            recievedData->setData("text/plain", revieved);
            dataType = QString("Unknown");
            emit DataSynchronized(recievedData, dataType);
            break;
        }
    }
}
