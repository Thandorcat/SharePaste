#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QUrl>
#include <QPixmap>

class Clipboard : public QObject
{
    Q_OBJECT
public:
    explicit Clipboard(QObject *parent = 0);
    QString GetClipboardText();
    QString GetClipboardFile();
    QPixmap GetClipboardPixmap();
    void SetClipboardText(QString newText);
    void SetClipboardFile(QString newFilePath);
    void SetClipboardPixmap(QPixmap nwePixmap);

signals:
    void ClipboardChanged();
public slots:
    void EmitChange();
private:
     QClipboard *clipboard;
};

#endif // CLIPBOARD_H
