#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QUrl>
#include <QImage>

class Clipboard : public QObject
{
    Q_OBJECT
public:
    explicit Clipboard(QObject *parent = 0);
    QString GetClipboardText();
    QString GetClipboardFile();
    QImage GetClipboardImage();
    QString GetClipboardType();
    void SetClipboardText(QString newText);
    void SetClipboardFile(QString newFilePath);
    void SetClipboardImage(QImage newImage);

signals:
    void ClipboardChanged();
public slots:
    void EmitChange();
private:
     QClipboard *clipboard;
     bool write;
};

#endif // CLIPBOARD_H
