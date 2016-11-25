#include "synchronization.h"

Synchronization::Synchronization(QObject *parent) : QObject(parent)
{
}

void Synchronization::SetServer(Server *serv)
{
    server = serv;
    connect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveData(QByteArray)));
}

void Synchronization::SendData()
{
    QStringList types;
    dataType = clipboard.GetClipboardType();
    types << "Text" << "File" << "Image"<<"Unknown";
    switch(types.indexOf(dataType))
    {
        case 0:
            byteData = clipboard.GetClipboardText().toUtf8();
            byteData.prepend('t');
            server->SendToServer(byteData);
            break;
        case 1:
            byteData = clipboard.GetClipboardText().toUtf8();
            byteData.prepend('f');
            server->SendToServer(byteData);
            break;
        case 2:
        {
            QImage image = clipboard.GetClipboardImage();
            QBuffer buff(&byteData);
            buff.open(QIODevice::WriteOnly);
            qDebug()<<image.save(&buff,"BMP");
            qDebug()<<byteData.size();
            byteData.prepend('i');
            server->SendToServer(byteData);
            break;
        }
        case 3:
            byteData = clipboard.GetClipboardText().toUtf8();
            byteData.prepend('u');
            server->SendToServer(byteData);
            break;
    }
}

void Synchronization::RecieveData(QByteArray revieved)
{
    qDebug()<<"Recieved data: ";
    qDebug()<<revieved;
    char type = revieved.at(0);
    revieved.remove(0,1);
    switch(type)
    {
        case 't':
        {
            qDebug()<<"Text";
            dataType = QString("Text");
            emit DataSynchronized(dataType);
            clipboard.SetClipboardText(QString(revieved));
            break;
        }
        case 'f':
        {
            qDebug()<<"File";
            dataType = QString("File");
            emit DataSynchronized(dataType);
            clipboard.SetClipboardText(QString(revieved));
            break;
        }
        case'i':
        {
            qDebug()<<"Image";
            QImage image;
            QBuffer buff(&revieved);
            buff.open(QIODevice::ReadOnly);
            qDebug()<<image.load(&buff,"BMP");
            qDebug()<<image.byteCount();
            qDebug()<<image.save("test.png","PNG",-1);
            dataType = QString("Image");
            emit DataSynchronized(dataType);
            clipboard.SetClipboardImage(image);
            break;
        }
        case 'u':
        {
            qDebug()<<"Unknown";
            dataType = QString("Unknown");
            emit DataSynchronized(dataType);
            clipboard.SetClipboardText(QString(revieved));
            break;
        }
    }
}
