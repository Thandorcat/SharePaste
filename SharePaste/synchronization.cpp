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
            SendFile(clipboard.GetClipboardFile());
            break;
        case 2:
        {
            QImage image = clipboard.GetClipboardImage();
            QBuffer buff(&byteData);
            buff.open(QIODevice::WriteOnly);
            qDebug()<<image.save(&buff,"PNG");
            qDebug()<<byteData.size();
            QByteArray info;
            info.append('i');
            info.append(QByteArray::number(byteData.size()));
            server->SendToServer(info);
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

void Synchronization::SendFile(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray info;
    info.append('f');
    info.append(QByteArray::number(file.size()));
    qDebug()<<QByteArray::number(file.size());
    info.append('f');
    QFileInfo fileInfo(file.fileName());
    QString name(fileInfo.fileName());
    info.append(name.toUtf8());
    server->SendToServer(info);
    byteData = file.read(1024*1024);
    byteData.prepend('#');
    server->SendToServer(byteData);
    while(!file.atEnd())
    {
        byteData = file.read(1024*1024);
        server->SendToServer(byteData);
    }
}

void Synchronization::RecieveData(QByteArray received)
{
    qDebug()<<endl<<"Recieved data: ";
    char type = received.at(0);
    received.remove(0,1);
    switch(type)
    {
        case 't':
        {
            qDebug()<<"Text";
            dataType = QString("Text");
            emit DataSynchronized(dataType);
            clipboard.SetClipboardText(QString(received));
            break;
        }
        case 'f':
        {
            qDebug()<<"File";            
            int fpos = received.indexOf('f');
            qDebug()<<received.left(fpos);
            dataSize = QString(received.left(fpos)).toInt();
            qDebug()<<"Size of file"<<dataSize;
            received.remove(0,fpos+1);
            fpos = received.indexOf('#');
            fileName = QString(received.left(fpos));
            qDebug()<<"File name: "<<fileName;
            received.remove(0,fpos+1);
            tempFile.setFileName(fileName);
            tempFile.open(QIODevice::WriteOnly);
            tempFile.write(received);
            if(tempFile.size()<dataSize)
            {
                disconnect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveData(QByteArray)));
                connect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveFile(QByteArray)));
            }
            else
            {
                emit DownloadStatusChanged(100);
                tempFile.close();
                dataType = QString("File");
                QFileInfo info(tempFile);
                emit DataSynchronized(dataType);
                clipboard.SetClipboardFile(info.absoluteFilePath());
            }
            break;
        }
        case'i':
        {
            qDebug()<<"Image";
            int numend = received.indexOf('i');
            dataSize = QString(received.left(numend)).toInt();
            qDebug()<<"Size of image"<<dataSize;
            received.remove(0,numend+1);
            byteData = received;
            dataBuff.setBuffer(&byteData);
            dataBuff.open(QIODevice::WriteOnly);
            dataBuff.write(received);
            if(byteData.size()<dataSize)
            {
                disconnect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveData(QByteArray)));
                connect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveImage(QByteArray)));
            }
            else
            {
                emit DownloadStatusChanged(100);
                dataBuff.close();
                dataBuff.open(QIODevice::ReadOnly);
                QImage image;
                image.load(&dataBuff,"PNG");
                qDebug()<<image.byteCount();
                dataType = QString("Image");
                emit DataSynchronized(dataType);
                qDebug()<<"Saving image";
                clipboard.SetClipboardImage(image);
                dataBuff.close();
                byteData.clear();
            }
            break;
        }
        case 'u':
        {
            qDebug()<<"Unknown";
            dataType = QString("Unknown");
            emit DataSynchronized(dataType);
            clipboard.SetClipboardText(QString(received));
            break;
        }
    }
}

void Synchronization::RecieveImage(QByteArray revievedData)
{
    dataBuff.write(revievedData);
    if(dataBuff.size()<dataSize)
    {
        emit DownloadStatusChanged((100*dataBuff.size())/dataSize);
    }
    else
    {
        emit DownloadStatusChanged(100);
        dataBuff.close();
        dataBuff.open(QIODevice::ReadOnly);
        QImage image;
        image.load(&dataBuff,"PNG");
        qDebug()<<image.byteCount();
        dataType = QString("Image");
        emit DataSynchronized(dataType);
        qDebug()<<"Saving image";
        clipboard.SetClipboardImage(image);
        disconnect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveImage(QByteArray)));
        connect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveData(QByteArray)));
        dataBuff.close();
        byteData.clear();
    }
}

void Synchronization::RecieveFile(QByteArray revievedData)
{
    tempFile.write(revievedData);
    if(tempFile.size()<dataSize)
    {
        emit DownloadStatusChanged((100*tempFile.size())/dataSize);
    }
    else
    {
        emit DownloadStatusChanged(100);
        tempFile.close();
        dataType = QString("File");
        QFileInfo info(tempFile);
        emit DataSynchronized(dataType);
        clipboard.SetClipboardFile(info.absoluteFilePath());
        disconnect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveFile(QByteArray)));
        connect(server, SIGNAL(BufferChanged(QByteArray)), this, SLOT(RecieveData(QByteArray)));
    }
}
