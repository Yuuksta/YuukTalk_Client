#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QWidget>
#include <QtNetwork>
#include <QMessageBox>

namespace Ui {
class FileReceiver;
}

class FileReceiver : public QWidget
{
    Q_OBJECT

public:
    explicit FileReceiver(QWidget *parent = 0);
    ~FileReceiver();
    QString FileName;
    QString FilePath;
    QString MyIP;
    void init();
public slots:
    void receiveData();


private:
    Ui::FileReceiver *ui;
    QFile file;
    QFileInfo fileinfo;
    int FileSize;
    int haveReceivedSize;
    QUdpSocket *udpSocket;
};

#endif // FILERECEIVER_H
