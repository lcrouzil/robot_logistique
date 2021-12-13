#include "client.h"
#include "ui_client.h"
#include <QMqttClient>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    this->m_client = new QMqttClient(this); //client
    this->m_client->setHostname(ui->le_host->text()); //hote
    this->m_client->setPort(ui->sb_port->value()); //port
    m_client->connectToHost();


    connect(this->ui->pb_co, SIGNAL(clicked()),this, SLOT(bp_co_clicked()));
}



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
    envoie.insert("color",ui->le_couleur->text());      //couleur
    envoie.insert("loadingArea",ui->sb_zdc->value());   //zone de chargement
    envoie.insert("depositArea",ui->sb_zdd->value());   //zone de dépot
    envoie.insert("robotId",ui->sb_idRobot->value());   // id_Robot

    this->emissionJson(envoie);

}

/**Envoie du JSon
 * @brief Client::emissionJson
 * @param envoie
 */
void Client::emissionJson(QJsonObject &envoie)
{

    //transformation de envoie en docJSON
    QJsonDocument docEnvoie(envoie);

    //transformation de envoie en Qstring
    QString strEnvoie(docEnvoie.toJson(QJsonDocument::Compact));

    //transformation de envoie en QByteArray
    QByteArray qbaEnvoie=strEnvoie.toUtf8();

    //envoie du message par la méthode publish
    m_client->publish(ui->le_sub->text(), qbaEnvoie,2 ,false);
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

