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
void Clipboard::SetClipboardPixmap(QPixmap newPixmap)
{
    clipboard->setPixmap(newPixmap);
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
QPixmap Clipboard::GetClipboardPixmap()
{
    return clipboard->pixmap();
}

QString Clipboard::GetClipboardFile()
{
    QString path = clipboard->text();
    path.remove(0,8);
    return path;
}

void Clipboard::EmitChange()
{
    qDebug()<<"Data changed!";
    emit ClipboardChanged();
}
