#ifndef FILESENDER_H
#define FILESENDER_H

#include <QWidget>
#include <QtNetwork>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class FileSender;
}

class FileSender : public QWidget
{
    Q_OBJECT

public:
    explicit FileSender(QWidget *parent = 0);
    ~FileSender();
    QString MyIP;
    QString IP;
    QFile file;

    void init();
    void sendData();

public slots:
    void receiveData();
    void readyToSendFile();
    void timeOut();

private:
    Ui::FileSender *ui;
    QUdpSocket *udpSocket;
    QByteArray dataBuffer;
    QTimer *timer;
    int sizeCount;

};

#endif // FILESENDER_H
