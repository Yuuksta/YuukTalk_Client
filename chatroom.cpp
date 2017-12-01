#include "chatroom.h"
#include "ui_chatroom.h"

ChatRoom::ChatRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatRoom)
{
    ui->setupUi(this);
}

ChatRoom::~ChatRoom()
{
    delete ui;
}

void ChatRoom::init(QTcpSocket *Socket_from,QString OfflineData){
    ui->textRoom->append(OfflineData);
    fileSender = new FileSender;
    fileReceiver = new FileReceiver;
    heIsOffline = 1;
    ImServer = 0;
    connect(ui->pushButton_sndmsg,SIGNAL(clicked(bool)),SLOT(sendMessage()));
    connect(ui->pushButton_connect,SIGNAL(clicked(bool)),SLOT(connectToOthers()));
    connect(ui->pushButton_snedFile,SIGNAL(clicked(bool)),SLOT(sndFile()));
    connect(ui->pushButton_recFile,SIGNAL(clicked(bool)),SLOT(recFile()));
    Socket = Socket_from;
    //ui->textRoom->append(Socket->localAddress().toString());
    qDebug() << Socket->localAddress().toString();
    connect(Socket,SIGNAL(readyRead()),SLOT(receiveData()));
    Server = new QTcpServer;
    Server->listen(QHostAddress::Any,6665);
    connect(Server,SIGNAL(newConnection()),SLOT(newConnection()));
}

void ChatRoom::sendMessage(){
    QString MsgToSend = ui->lineEdit_Msg->text();
    if(MsgToSend != ""){
        if(!ImServer){
            if(heIsOffline){ //connect to server
                Socket->abort();
                Socket->connectToHost(QHostAddress(HostIP),6666);
            }
            else{ // connect to him
                Socket->abort();
                Socket->connectToHost(QHostAddress(IP),6665);
            }
            if(heIsOffline){
                ui->textRoom->append(ID + ": " + MsgToSend);
                MsgToSend = ("offline|" + ID + "|to|" + ui->lineEdit_ID->text() + "|" + (MsgToSend));
            }
            else{
                MsgToSend = (ID + ": " + MsgToSend);
                ui->textRoom->append(MsgToSend);
            }
        }
        else{
            MsgToSend = (ID + ": " + MsgToSend);
            ui->textRoom->append(MsgToSend);
        }
        Socket->write(MsgToSend.toLocal8Bit());
        ui->lineEdit_Msg->setText("");
    }
}

void ChatRoom::receiveData(){
    QByteArray data = Socket->readAll();
    QString DataString = data;
    if(DataString.section("|",0,0) == "IP"){
        IP = DataString.section("|",1,1);
        Socket->abort();
        Socket->connectToHost(QHostAddress(IP),6665);
        ui->textRoom->append(DataString);
        ui->textRoom->append("Successfully connected");
        heIsOffline = 0;
    }
    else if(DataString.contains("is offline!")){
        heIsOffline = 1;
        ui->textRoom->append(ui->lineEdit_ID->text() + " is offline");
    }
    else if(DataString.section("|",0,0) == "your friend"){
        ImServer = 0;
        heIsOffline = 1;
        HisID = DataString.section("|",1,1);
        Socket->abort();
        Socket->connectToHost(QHostAddress(HostIP),6666);
        Socket->write(HisID.toLocal8Bit()+"|offline");
    }
    else if(DataString == "readyToSendFile"){
        ui->textRoom->append("your friend want to send file\nplease click ReceiveFile button");
    }
    else{
        ui->textRoom->append(data);
    }
}

void ChatRoom::showSuccess(){
    Msg = "";
    Msg = Msg + "Successfully connected to server!";
    ui->textRoom->setText(Msg);
    ui->lineEdit_ID->setText("");
}

void ChatRoom::newConnection(){
    Socket = Server->nextPendingConnection();
    connect(Socket,SIGNAL(readyRead()),SLOT(receiveData()));
    IP = Socket->peerAddress().toString();
    //ui->textRoom->append("Someone is connected");
    ImServer = 1;
}

void ChatRoom::closeEvent(QCloseEvent *event){
    Socket->abort();
    Socket->connectToHost(QHostAddress(HostIP),6666);
    Socket->write(ID.toLocal8Bit()+"|offline");
}

void ChatRoom::connectToOthers(){
    if(ui->lineEdit_ID->text() != ""){
        Socket->abort();
        Socket->connectToHost(QHostAddress(HostIP),6666);
        QString OthersID = "ConnectTo|";
        OthersID.append(ui->lineEdit_ID->text());
        Socket->write(OthersID.toLocal8Bit());
        //ui->textRoom->append(OthersID);
    }

}
void ChatRoom::sndFile(){
    QString a = "readyToSendFile";
    Socket->write(a.toLocal8Bit());
    fileSender->show();
    fileSender->MyIP =MyIP;
    fileSender->IP = IP;
    fileSender->init();
}

void ChatRoom::recFile(){
    fileReceiver->MyIP = MyIP;
    fileReceiver->show();
    fileReceiver->init();
}

