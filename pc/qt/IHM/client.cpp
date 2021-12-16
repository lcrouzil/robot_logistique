#include "client.h"
#include "ui_client.h"
#include <QMqttClient>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);



    connect(this->ui->pb_co, SIGNAL(clicked()),this, SLOT(bp_co_clicked()));
}


/**destructeur
 * @brief Client::~Client
 */
Client::~Client()
{
    delete ui;
}

/**Envoie du message d'ordre
 *
 * @brief Client::jsonMessageOrder
 */
void Client::jsonMessageOrder()
{
    QJsonObject envoie;
    QString topic;
    topic = "field/ui/<id>/ordre";
    quint8 qos;
    qos=2;
    bool retain;
    retain = false;

    envoie.insert("color",ui->le_couleur->text());      //couleur
    envoie.insert("loadingArea",ui->sb_zdc->value());   //zone de chargement
    envoie.insert("depositArea",1);                     //zone de dépot fixé
    envoie.insert("robotId","ROBOT5");                  // id_Robot

    this->emissionJson(envoie,topic,qos,retain);

}

/**chemin a effectuer
 * @brief Client::jsonMessagePath
 */
void Client::jsonMessagePath()
{
    QJsonObject envoie;
    QString topic;
    topic = "field/robot/<id_robot>/path";
    quint8 qos;
    qos=2;
    bool retain;
    retain = false;

    //payload: {“id”: <id_etape>, “direction”: <char_direction(L/F/R)>}
    envoie.insert("id","id_etape");
    envoie.insert("direction","char_dir");

    this->emissionJson(envoie,topic,qos,retain);

}

/**Envoi messageCam pour lancer le scan
 * @brief Client::jsonMessageCam
 */
void Client::jsonMessageCam()
{
    QJsonObject envoie;
    QString topic;
    topic = "field/camera/5/scan";
    quint8 qos;
    qos=1;
    bool retain;
    retain = true;

    envoie.insert("robot","ROBOT5");
    this->emissionJson(envoie,topic,qos,retain);
}
/**Interface indique la couleur dans la zone determinee
 * @brief Client::jsonMessageColor
 */
void Client::jsonMessageAreaColor()
{
    QJsonObject envoie;
    QString topic;
    int zoneDeChargement = ui->sb_zdc->value();
    QString zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";
    quint8 qos;
    qos=0;
    bool retain;
    retain = true;
    envoie.insert("color","color");
    this->emissionJson(envoie,topic,qos,retain);

}
/**Envoie du JSon
 * @brief Client::emissionJson
 * @param envoie
 */
void Client::emissionJson(QJsonObject &envoie, QString &topic, quint8 &qos, bool &retain)
{

    //transformation de envoie en docJSON
    QJsonDocument docEnvoie(envoie);

    //transformation de envoie en Qstring
    QString strEnvoie(docEnvoie.toJson(QJsonDocument::Compact));

    //transformation de envoie en QByteArray
    QByteArray qbaEnvoie=strEnvoie.toUtf8();

    //envoie du message par la méthode publish
    m_client->publish(topic, qbaEnvoie, qos, retain);

}



/**bouton de co/deco avec modification du bouton
 *
 * @brief Client::bp_co_clicked
 */
void Client::bp_co_clicked()
{
    this->m_client->setHostname(ui->le_host->text()); //hote
    this->m_client->setPort(ui->sb_port->value()); //port

    m_client->connectToHost();
    /*
    //verifie etat du client
        //si client co
    if (m_client->state() == QMqttClient::Disconnected)
    {
        ui->le_host->setEnabled(false);
        ui->pb_co->setEnabled(false);
        ui->le_host->setText(tr("Disconnect"));
        m_client->connectToHost();

        //si client non co
    } else
    {
        ui->le_host->setEnabled(true);
        ui->pb_co->setEnabled(true);
        ui->le_host->setText(tr("Connect"));
        m_client->disconnectFromHost();
    }
    */
}

