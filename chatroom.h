#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QtNetwork>
#include <QFileDialog>
#include "filesender.h"
#include "filereceiver.h"

namespace Ui {
class ChatRoom;
}

class ChatRoom : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoom(QWidget *parent = 0);
    ~ChatRoom();
    void init(QTcpSocket *Socket_from,QString OfflineData);
    bool heIsOffline;
    bool ImServer;
    QString ID;
    QString HostIP;
    QString MyIP;
    QString HisID;

public slots:
    void receiveData();
    void sendMessage();
    void connectToOthers();
    void showSuccess();
    void newConnection();
    void sndFile();
    void recFile();

private:
    Ui::ChatRoom *ui;
    QTcpSocket *Socket;
    QTcpServer *Server;
    QUdpSocket *UdpSocket;
    QByteArray Msg;
    QFile File;
    QString IP;
    QHostInfo info;
    FileSender *fileSender;
    FileReceiver *fileReceiver;

protected:
    void closeEvent(QCloseEvent *event);
};


#endif // CHATROOM_H
