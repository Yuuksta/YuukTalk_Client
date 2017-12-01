#include "signuproom.h"
#include "ui_signuproom.h"

signuproom::signuproom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signuproom)
{
    ui->setupUi(this);
    connect(ui->pushButton_SignUp,SIGNAL(clicked(bool)),SLOT(toSignUp()));
    connect(ui->pushButton_FindPsw,SIGNAL(clicked(bool)),SLOT(findPassword()));
}

signuproom::~signuproom()
{
    Socket->close();
    delete ui;
}

void signuproom::toSignUp(){
    QByteArray ID = ui->lineEdit_ID_SignUp->text().toLocal8Bit();
    QByteArray Password = ui->lineEdit_Psw->text().toLocal8Bit();
    QByteArray ConfirmPassword = ui->lineEdit_CPsw->text().toLocal8Bit();
    if(Password != ConfirmPassword){
        QMessageBox::critical(NULL,"warning","\nTwo Password doesn't the same\n");
    }
    else{
        Socket = new QTcpSocket;
        Socket->connectToHost(QHostAddress(HostIP),6666);
        connect(Socket,SIGNAL(readyRead()),SLOT(ProcessSignUp()));
        QByteArray MsgToSend;
        MsgToSend.append(ID);
        MsgToSend.append("|");
        MsgToSend.append(Password);
        MsgToSend.append("|");
        MsgToSend.append("signup");
        Socket->write(MsgToSend);
    }
}


void signuproom::ProcessSignUp(){
    QByteArray Data = Socket->readAll();
    if(Data.contains("full")){
        QMessageBox::critical(NULL,"who0ops","\nfailed to sign up\n");
    }
    else if(Data.contains("already")){
             QMessageBox::critical(NULL,"whooops","\nYour ID has been registed\n");
         }
    else QMessageBox::information(NULL,"","\nSuccessfully signed up,\nplease remember your ID and Password\n");
    Socket->close();
}

void signuproom::findPassword(){
     Socket = new QTcpSocket;
     QByteArray ID = ui->lineEdit_ID_FindPsw->text().toLocal8Bit();
     Socket->connectToHost(QHostAddress(HostIP),6666);
     connect(Socket,SIGNAL(readyRead()),this,SLOT(ProcessFindPassword()));
     QByteArray MsgToSend;
     MsgToSend.append(ID);
     MsgToSend.append("|");
     MsgToSend.append("findPassword");
     Socket->write(MsgToSend);
}

void signuproom::ProcessFindPassword(){
    QByteArray Data = Socket->readAll();
    if(Data.contains("have not found")){
        QMessageBox::critical(NULL,"who0ops","\nID has not been registed\n");
    }
    else{
        QMessageBox::information(NULL,"","\nYour Passoword is:\n"+Data);
    }
}
