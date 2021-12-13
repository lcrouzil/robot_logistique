#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
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

    void jsonMessageOrder();
    void emissionJson(QJsonObject &);
    //QMqttTopicName &topicOrder;




private:
    Ui::Client *ui;
    QMqttClient *m_client;

public slots:
    void bp_co_clicked();

};
#endif // CLIENT_H
