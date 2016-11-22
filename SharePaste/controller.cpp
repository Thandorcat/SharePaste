#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    connect(&clipboard, SIGNAL(ClipboardChanged(QString)), this, SLOT(UpdateClipboardField(QString)));
    connect(&clipboard, SIGNAL(ClipboardChanged(QString)), this, SLOT(SyncClipboard()));
    connect(&sync, SIGNAL(DataSynchronized(QMimeData*,QString)),
            this, SLOT(ServerBufferSyncronized(QMimeData*,QString)));
    sync.SetServer(&server);
}

void Controller::SetMainwindow(MainWindow *w)
{
    window = w;
    qDebug()<<"Controller created!";
    connect(window, SIGNAL(ConnectRequest(QString,quint16)),
            this, SLOT(ConnectAttempt(QString,quint16)));
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
            window->SetClipboardField("Bitmap image");
            break;
        case 3:
            window->SetClipboardField("Unknown: "+clipboard.GetClipboardText());
            break;
    }
}

void Controller::SyncClipboard()
{
    if(!writeFlag)
    {
        sync.SetData(clipboard.GetClipboardMime(), clipboard.GetClipboardType());
        sync.SendData();
    }
    else
        writeFlag = false;
}

void Controller::ConnectAttempt(QString adress, quint16 port)
{
    server.ConnectToServer(adress, port);
}

void Controller::ServerBufferSyncronized(QMimeData* newData, QString Type)
{
    qDebug()<<"Updating clipboard from server!"<<Type;
    writeFlag = true;
    clipboard.SetClipboardMime(newData);
}


