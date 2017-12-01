#include "filereceiver.h"
#include "ui_filereceiver.h"

FileReceiver::FileReceiver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileReceiver)
{
    ui->setupUi(this);
}

FileReceiver::~FileReceiver()
{
    delete ui;
}


void FileReceiver::init(){
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress(MyIP), 6656);
    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(receiveData()));
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    haveReceivedSize = 0;
}

void FileReceiver::receiveData(){
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    udpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
    QString datagramString = datagram;
    if(datagramString.section("|",0,0) == "fileName"){
        FilePath = datagramString.section("|",2,2);
        fileinfo.setFile(FilePath);
        FileName = fileinfo.fileName();
        FileSize = datagramString.section("|",1,1).toInt();
        ui->progressBar->setMaximum(FileSize);
        qDebug() << FileSize;
        file.setFileName(FileName);
        file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered);
        udpSocket->writeDatagram("2",1,sender,6655);
        //qDebug() << "Write 2";
    }
    else if(datagramString == "complete") {
        udpSocket->abort();
        QMessageBox::information(NULL,"","\nFile Transformed Successfully!\n");
        file.close();
        this->hide();
    }
    else{
        file.write(datagram.data(),datagram.size());
        haveReceivedSize += datagram.size()/1000;
        ui->progressBar->setValue(haveReceivedSize);
        qDebug() << "received:" << haveReceivedSize;
        udpSocket->writeDatagram("1",1,sender,6655);
    }
}
