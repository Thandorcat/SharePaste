#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    connect(&clipboard, SIGNAL(ClipboardChanged(QString)),
            this, SLOT(UpdateClipboardField(QString)));
    connect(&clipboard, SIGNAL(ClipboardChanged(QString)),
            this, SLOT(SyncClipboard()));
    connect(&sync, SIGNAL(DataSynchronized(QString)),
            this, SLOT(ServerBufferSyncronized(QString)));
    connect(&sync, SIGNAL(DownloadStatusChanged(int)),
            this, SLOT(DownloadStatus(int)));
    sync.SetServer(&server);
}

void Controller::SetMainwindow(MainWindow *w)
{
    window = w;
    qDebug()<<"Controller created!";
    connect(window, SIGNAL(ConnectRequest(QString,quint16)),
            this, SLOT(ConnectAttempt(QString,quint16)));
    connect(window, SIGNAL(Disconnect()),
            this, SLOT(Disonnect()));
    UpdateClipboardField(clipboard.GetClipboardType());
}

void Controller::UpdateClipboardField(QString dataType)
{
    qDebug()<<"Updating clipboard from local.";
    QStringList types;
    types << "Text" << "File" << "Image"<<"Unknown";

    switch(types.indexOf(dataType))
    {
        case 0:
            window->SetClipboardField("Text: "+clipboard.GetClipboardText());
            break;
        case 1:
            window->SetClipboardField("File: "+clipboard.GetClipboardFile());
            break;
        case 2:
            window->SetClipboardField("Bitmap image: "+QString::number(clipboard.GetClipboardImage().byteCount()));
            break;
        case 3:
            window->SetClipboardField("Unknown: "+clipboard.GetClipboardText());
            break;
    }
}

void Controller::DownloadStatus(int value)
{
    window->DownloadStatus(value);
}

void Controller::SyncClipboard()
{
    if(!writeFlag&&connected)
    {
        qDebug()<<"Syncronizing data!";
        sync.SendData();
    }
    else
        writeFlag = false;
}

void Controller::ConnectAttempt(QString adress, quint16 port)
{
    server.ConnectToServer(adress, port);
    connected = true;
    SyncClipboard();
}

void Controller::Disonnect()
{
    connected = false;
    server.DisconnectFromServer();
}

void Controller::ServerBufferSyncronized(QString Type)
{
    qDebug()<<"Updating clipboard from server!"<<Type;
    writeFlag = true;
}


