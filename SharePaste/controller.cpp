#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    connect(&clipboard, SIGNAL(ClipboardChanged()), this, SLOT(UpdateClipboardField()));
}

void Controller::SetMainwindow(MainWindow *w)
{
    window = w;
    qDebug()<<"Controller created";
    connect(window, SIGNAL(ConnectRequest(QString,quint16)),
            this, SLOT(ConnectAttempt(QString,quint16)));
}

void Controller::UpdateClipboardField()
{
    qDebug()<<"Updating clipboard field";
    QString dataType = clipboard.GetClipboardType();
    QStringList types;
    types << "Text" << "File" << "Image"<<"Unknown";

    switch(types.indexOf(dataType))
    {
        case 0:
            window->SetClipboardField("Text: "+clipboard.GetClipboardText());
            server.sendToServer(clipboard.GetClipboardText());
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
void Controller::ConnectAttempt(QString adress, quint16 port)
{
    server.connectToServer(adress, port);
}


