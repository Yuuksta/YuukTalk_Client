#include "mainwindow.h"
#include "ui_mainwindow.h"

#define login 1
#define nologin 0
#define reg 2
int state = nologin;
QByteArray falseArray = "false";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    HostIP = "10.11.58.84";
    room = new ChatRoom;
    signup = new signuproom;
    //MyIP = getIP();
    connect(ui->pushButton_Login,SIGNAL(clicked(bool)),SLOT(logIn()));
    connect(ui->pushButton_Signup,SIGNAL(clicked(bool)),SLOT(signUp()));
}

MainWindow::~MainWindow()
{
    delete ui;
    Socket->write("offline");
}

void MainWindow::logIn(){
    ID = ui->ID_text->text();
    QByteArray Password = ui->Password_text->text().toLocal8Bit();
    Socket = new QTcpSocket;
    Socket->connectToHost(QHostAddress(HostIP),6666);
    dis = connect(Socket,SIGNAL(readyRead()),SLOT(ProcesslogIn()));
    QByteArray MsgToSend;
    if(ID != "" && Password != ""){
        MsgToSend.append(ID.toLocal8Bit());
        MsgToSend.append("|");
        MsgToSend.append(Password);
        MsgToSend.append("|");
        MsgToSend.append("login");
        //MsgToSend.append("|");
        //MsgToSend.append(MyIP.toLocal8Bit());
        Socket->write(MsgToSend);
    }
}

void MainWindow::signUp(){
   // this->hide();
    signup->show();
    signup->HostIP = HostIP;
}

void MainWindow::ProcesslogIn(){
    if(state != login){
        QString DataString  = Socket->readAll();
        if(DataString.contains("ERROR")){
            QMessageBox::critical(NULL,"whooops","\nID or Password incorrect\n");
        }
        else{
            Socket->write(ID.toLocal8Bit() + "|IP|" + Socket->localAddress().toString().toLocal8Bit());
            this->hide();
            OfflineData = DataString.section("|",1,1);
            disconnect(dis);
            room->ID = ID;
            room->MyIP = Socket->localAddress().toString();
            room->HostIP = HostIP;
            room->show();
            room->init(Socket,OfflineData);
        }
    }
}
