 #ifndef CLIENT_H
#define CLIENT_H

//*******************************************************************************************************************
//          INCLUDE                                                                                              *
//*******************************************************************************************************************

#include <QMainWindow>
#include <QDebug>
#include<QJsonObject>
#include <QJsonArray>
#include<QJsonDocument>
#include <QMqttClient>
#include <QObject>
#include <QArrayData>
#include<QByteArray>


QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

    void emissionJson(QJsonObject &, QString &,quint8 &, bool &);

    int jsonMessagePath(int);
    void jsonMessageCam();
    void jsonMessageAreaColor();


    //void jsonMessageReception();
   // void setSubCam(QMqttClient);



private:
    Ui::Client *ui;
    QMqttClient *m_client;
    QMqttSubscription * subscription;
public slots:
    void bp_co_clicked();

    //void on_simuSTM_clicked();

    void scan();
    void checketagere();
    void setsub();
    void jsonMessageOrder();
    void messReceivedCam(QMqttMessage);
    void messReceivedRobStatus(QMqttMessage);
    void messReceivedRobButton(QMqttMessage);
    void messReceiveEtagere1(QMqttMessage);
    void messReceiveEtagere2(QMqttMessage);
    void messReceiveEtagere3(QMqttMessage);
    void messReceiveEtagere4(QMqttMessage);
    void updatestatusetagere();
    void setdiscon();
    void dissub();
    void checkexspin(int);
    void checkexcombo(int);






};
#endif // CLIENT_H
