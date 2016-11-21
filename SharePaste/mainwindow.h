#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void SetClipboardField(QString newText);
    ~MainWindow();
private slots:
    void ConnectPressed();

signals:
    void ConnectRequest(QString adress, quint16 port);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H