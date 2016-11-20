#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "clipboard.h"
#include "mainwindow.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    void SetMainwindow(MainWindow *w);
signals:

private slots:
    void UpdateClipboardField();
private:
    Clipboard clipboard;
    MainWindow *window;
};

#endif // CONTROLLER_H
