#include "filesender.h"
#include "ui_filesender.h"

FileSender::FileSender(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileSender)
{
    ui->setupUi(this);

}

FileSender::~FileSender()
{
    delete ui;
}

void FileSender::init(){
    udpSocket = new QUdpSocket;
    udpSocket->bind(QHostAddress(MyIP), 6655);
    timer = new QTimer;
    sizeCount = 0;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(receiveData()));
    connect(ui->pushButton_chooseFile,SIGNAL(clicked(bool)),SLOT(readyToSendFile()));
    connect(timer,SIGNAL(timeout()),SLOT(timeOut()));
}

void FileSender::readyToSendFile(){
    QString fileName = QFileDialog::getOpenFileName();
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);
    long fileSize = file.size()/1000;
    ui->progressBar->setMaximum(file.size()/1000);
    QString fileSizeString = QString::number(fileSize,10);
    QString fileName_Sender = "fileName|" + fileSizeString + "|" + fileName;
    dataBuffer = fileName_Sender.toLocal8Bit();
    udpSocket->writeDatagram(fileName_Sender.toLocal8Bit(),QHostAddress(IP),6656);
    timer->start(500);
}

void FileSender::receiveData(){
    qDebug() << "Here comes the data";
    if(udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
        qDebug() << datagram;
        if(datagram == "2"){
            sendData();
            qDebug() << "receive 2";
        }
        if(datagram == "1"){
            sizeCount += 8;
            ui->progressBar->setValue(sizeCount);
            qDebug() << "FileSend: "<< sizeCount;
            sendData();
        }
        if(sizeCount > file.size()/1000){
            QString completeString = "complete";
            udpSocket->writeDatagram(completeString.toLocal8Bit(),QHostAddress(IP),6656);
            udpSocket->close();
            QMessageBox::information(NULL,"","\nFile Transformed Successfully!\n");
            file.close();
            timer->stop();
        }
     }
}

void FileSender::sendData(){
    if (!file.atEnd()) {
        QByteArray data = file.read(8000);
        dataBuffer = data;
        udpSocket->writeDatagram(data,QHostAddress(IP),6656);
        qDebug() << "sendData";
        timer->start(500);
    }
}

void FileSender::timeOut(){
    qDebug() << "timeout";
    udpSocket->writeDatagram(dataBuffer,QHostAddress(IP),6656);
    timer->start(500);
}
