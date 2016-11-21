#include "clipboard.h"

Clipboard::Clipboard(QObject *parent) : QObject(parent)
{
    clipboard = QApplication::clipboard();
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(EmitChange()));
    qDebug()<<"Clipboard created!";
}
void Clipboard::SetClipboardText(QString newText)
{
    clipboard->setText(newText);
}
void Clipboard::SetClipboardImage(QImage newImage)
{
    clipboard->setImage(newImage);
}


void Clipboard::SetClipboardFile(QString newFilePath)
{
    QMimeData * md = new QMimeData();
    QUrl url = QUrl::fromLocalFile(newFilePath);
    if ( url.isValid() )
    {
        QList<QUrl> list;
        list.append( url );
        md->setUrls(list);
        QApplication::clipboard()->setMimeData( md );
    }
}
QString Clipboard::GetClipboardText()
{
    return clipboard->text();
}
QImage Clipboard::GetClipboardImage()
{
    return clipboard->image();
}

QString Clipboard::GetClipboardFile()
{
    QString path = clipboard->text();
    path.remove(0,8);
    return path;
}

QString Clipboard::GetClipboardType()
{
    const QMimeData *data = clipboard->mimeData();
    if(data->hasImage())
        return QString("Image");
    if(data->hasUrls())
        return QString("File");
    if(data->hasText())
       return QString("Text");
    return QString("Unknown");
}

void Clipboard::EmitChange()
{
    qDebug()<<endl<<"Data changed!";
    qDebug()<<GetClipboardType();
    //qDebug()<<"Copied types: "<<clipboard->mimeData()->formats();
    emit ClipboardChanged();
}
