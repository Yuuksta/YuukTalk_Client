#ifndef SIGNUPROOM_H
#define SIGNUPROOM_H

#include <QWidget>
#include <QtNetwork>
#include <QMessageBox>
namespace Ui {
class signuproom;
}

class signuproom : public QWidget
{
    Q_OBJECT

public:
    explicit signuproom(QWidget *parent = 0);
    ~signuproom();
    QString HostIP;

public slots:
    void toSignUp();
    void findPassword();
    void ProcessSignUp();
    void ProcessFindPassword();
private:
    Ui::signuproom *ui;

    QTcpSocket *Socket;
};

#endif // SIGNUPROOM_H
