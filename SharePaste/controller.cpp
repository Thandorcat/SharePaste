#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    connect(&clipboard, SIGNAL(ClipboardChanged()), this, SLOT(UpdateClipboardField()));
}

void Controller::SetMainwindow(MainWindow *w)
{
    window = w;
    qDebug()<<"Controller created";
}

void Controller::UpdateClipboardField()
{
    qDebug()<<"Updating clipboard field";
    QString clipboardData = clipboard.GetClipboardText();
    window->SetClipboardField(clipboardData);
}


