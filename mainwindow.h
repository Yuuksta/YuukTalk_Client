#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatroom.h"
#include "signuproom.h"
#include <QMessageBox>
#include <QTimer>
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTcpSocket *Socket;
    QString ID;
    QString HostIP;
    QString MyIP;
    void encrypt();
    void enterChatRoom();
    QString getIP();
    QMetaObject::Connection dis;
    QString OfflineData;

public slots:
    void logIn();
    void signUp();
    void ProcesslogIn();

private:
    Ui::MainWindow *ui;
    ChatRoom *room;
    signuproom *signup;
};

#endif // MAINWINDOW_H
