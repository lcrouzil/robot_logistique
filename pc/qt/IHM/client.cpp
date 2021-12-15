#include "client.h"
#include "ui_client.h"
#include <QMqttClient>

QMap<int,QString> coletagere; //couleur dans les etageres

int id;  //declaration de notre id d'etape

//chemin pour chaque cas
QMap<int,QString> pazr; //chemin zone
QMap<int,QString> pazc1;//chemin zone de chargement
QMap<int,QString> pazc2;// chemin zone de chargement
QMap<int,QString> pazc3;
QMap<int,QString> pazrd;
QMap<int,QString> pazrcr;


QString colorballtargeted;

int receptz=0;
bool colok;
bool finishloop=false;
bool ballonbot;


/**verification de la cible dans l'etagere
 * @brief checketagere
 */
void checketagere()
{
    bool ballfound=false;
    qDebug()<<colorballtargeted;

    if(coletagere.value(1)==colorballtargeted){
            ballfound=true;
            receptz=1;
     }else if(coletagere.value(2)==colorballtargeted){
            ballfound=true;
            receptz=2;
     }else if(coletagere.value(3)==colorballtargeted){
        ballfound=true;
        receptz=3;
     }else if(coletagere.value(4)==colorballtargeted){
        ballfound=true;
        receptz=4;

    }

    if(ballfound==false){
        receptz=receptz%4;
        receptz++;
    }
}


/**constructeur de client
 * @brief Client::Client
 * @param parent
 */
Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    //id de notre zone de depart --> zone de depot 1
    id=12;
    //mise en place des deplacement a effectuer en fonctions de l'id
    pazr.insert(0,"L");
    pazr.insert(1,"F");
    pazr.insert(2,"F");
    pazr.insert(3,"F");
    pazr.insert(4,"F");
    pazc1.insert(5,"L");
    pazc1.insert(6,"F");
    pazc1.insert(7,"F");
    pazc1.insert(8,"F");
    pazc2.insert(5,"L");
    pazc2.insert(6,"R");
    pazc2.insert(7,"F");
    pazc2.insert(8,"F");
    pazc3.insert(6,"R");
    pazrd.insert(9,"F");
    pazrd.insert(10,"F");
    pazrd.insert(11,"F");
    pazrd.insert(12,"F");
    pazrcr.insert(9,"R");
    pazrcr.insert(1,"R");

    ui->pb_order->setEnabled(false);
    //initialise la co
    connect(this->ui->pb_co, SIGNAL(clicked()),this, SLOT(bp_co_clicked()));

    //simulation du bouton de la stm
    connect(this->ui->pushButton, SIGNAL(clicked()),this, SLOT(scan()));

    //envoie de l'odre

    connect(this->ui->pb_order, SIGNAL(clicked()),this, SLOT(jsonMessageOrder()));

    // initialisation de l'affichage
    coletagere.insert(1,"NoA");
    coletagere.insert(2,"NoA");
    coletagere.insert(3,"NoA");
    coletagere.insert(4,"NoA");



    //recuperation des infos sur les etageres
    updatestatusetagere();

}


/**destructeur
 * @brief Client::~Client
 */
Client::~Client()
{
    delete ui;
}

//*******************************************************************************************************************
//          PARTIE PUB                                                                                              *
//*******************************************************************************************************************


/**Envoie du message d'ordre
 *
 * @brief Client::jsonMessageOrder
 */
void Client::jsonMessageOrder()
{
    QJsonObject envoie;
    QString topic;
    topic = "field/ui/5/ordre";
    quint8 qos;
    qos=2;
    bool retain;
    retain = false;
    receptz = ui->sb_zdc->value();
    qDebug()<<receptz;
    colorballtargeted = ui->le_couleur->text();    
    envoie.insert("color",ui->le_couleur->text());      //couleur
    envoie.insert("loadingArea",ui->sb_zdc->value());   //zone de chargement
    envoie.insert("depositArea",1);                     //zone de dépot fixé
    envoie.insert("robotId","ROBOT5");                  // id_Robot

    this->emissionJson(envoie,topic,qos,retain);

}


/**chemin a effectuer
 * Algo tres long prevoir aspirine
 * @brief Client::jsonMessagePath
 */
int Client::jsonMessagePath(int id)
{
    qDebug()<<"bb";
    qDebug()<<receptz;
    QJsonObject envoie;
    QString topic;
    topic = "field/robot/ROBOT5/path";
    quint8 qos;
    qos=2;
    bool retain;
    retain = false;
    if (id<8)
    {
       finishloop=false;
    }

    if(id<receptz)
    {
        id++;
        pazr.value(id);

        //publish id et sa value
        envoie.insert("id",id);
        envoie.insert("direction", pazr.value(id));

    }
    else if(id==receptz)
    {
        //ne rien faire
    }
    else if (id<9)
    {
        if(id<4)
        {
            id=4;
        }
        if(receptz==1)
        {
            if (id==8)
            {
                    finishloop=true;
            }
            else
            {
                id++;
                pazc1.value(id);

                //publish id et sa value
                envoie.insert("id",id);
                envoie.insert("direction", pazc1.value(id));
            }
        }
        if(receptz==2)
        {
            if (id==8)
            {
                finishloop=true;
            }
            else
            {
                id++;
                pazc2.value(id);

                //publish id et sa value
                envoie.insert("id",id);
                envoie.insert("direction", pazc2.value(id));

            }
        }
        if(receptz==3)
        {
            if (id==7)
            {
                finishloop=true;
            }
            else
            {
                id++;
                pazc2.value(id);

                //publish id et sa value
                envoie.insert("id",id);
                envoie.insert("direction", pazc2.value(id));
            }
        }
        if(receptz==4)
        {
            if (id==6)
            {
                finishloop=true;
            }else
            {
                id++;
                pazc3.value(id);

                //publish a et la value
                envoie.insert("id",id);
                envoie.insert("direction", pazc3.value(id));

            }
        }
    }
    if(finishloop==true)
    {
        if(id<8)
        {
                id=8;//avant7
        }
        if(colok==true)
        {
            id++;
            qDebug()<<"testaa";
            qDebug()<<id;
            pazrd.value(id);
            envoie.insert("id",id);
            envoie.insert("direction", pazrd.value(id));

        }else
        {
            id++;
            qDebug()<<"testbb";
            if(id==10)
            {
                id=1;
                if(ballonbot==false)
                {
                    checketagere();
                }
                envoie.insert("id",id);
                envoie.insert("direction", pazrcr.value(id));

            }
            else
            {
                pazrcr.value(id);
                envoie.insert("id",id);
                envoie.insert("direction", pazrcr.value(id));
            }
        }


    }
    qDebug()<<"aaaa";
    if (id<13)
    {
        this->emissionJson(envoie,topic,qos,retain);
        return id;
    }
    else
    {
        return 12;
    }
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
/*
void Client::jsonMessageAreaColor()
{
    QJsonObject envoie;
    QString topic;
    int zoneDeChargement = ui->sb_zdc->value();
    QString zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";    // verifier le topic =========//
    quint8 qos;
    qos=0;
    bool retain;
    retain = true;
    envoie.insert("color","color");
    this->emissionJson(envoie,topic,qos,retain);/
}
*/

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

/**lancement de l'ordre scan
 * @brief Client::scan
 */
void Client::scan()
{
    QString topic="field/camera/5/scan";
    QJsonObject checkObject;
    checkObject.insert("robot","ROBOT5");
    QJsonDocument jsDoc(checkObject);
    QString jsString = QString::fromLatin1(jsDoc.toJson());
    m_client->publish(topic, jsString.toUtf8(),2);
}


//*******************************************************************************************************************
//          PARTIE SUB                                                                                              *
//*******************************************************************************************************************

/**creation des sub
 * @brief Client::setsub
 */
void Client::setsub()
{
    /*
    QJsonObject envoie;
    QString topic;
    int zoneDeChargement = 1;
    QString zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";    // verifier le topic =========//
    quint8 qos;
    qos=0;
    bool retain;
    retain = true;
    envoie.insert("color","NoA");
    this->emissionJson(envoie,topic,qos,retain);

     zoneDeChargement = 2;
    zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";    // verifier le topic =========//

    qos=0;
    retain = true;
    envoie.insert("color","NoA");
    this->emissionJson(envoie,topic,qos,retain);
    zoneDeChargement = 3;
   zdc = QString::number(zoneDeChargement);
   topic = "field/loading_area/"+ zdc +"/color";    // verifier le topic =========//

   qos=0;
   retain = true;
   envoie.insert("color","NoA");
   this->emissionJson(envoie,topic,qos,retain);
   zoneDeChargement = 4;
  zdc = QString::number(zoneDeChargement);
  topic = "field/loading_area/"+ zdc +"/color";    // verifier le topic =========//

  qos=0;
  retain = true;
  envoie.insert("color","NoA");
  this->emissionJson(envoie,topic,qos,retain);
  */

    QString sub="field/camera/5/color";
    subscription = m_client->subscribe(sub,1);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedCam(QMqttMessage)));

    sub="field/robot/ROBOT5/status";
    subscription = m_client->subscribe(sub,1);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedRobStatus(QMqttMessage)));

    sub="field/robot/ROBOT5/button";
    subscription = m_client->subscribe(sub,2);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedRobButton(QMqttMessage)));

    sub="field/loading_area/1/color";
    subscription = m_client->subscribe(sub,0);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere1(QMqttMessage)));

    sub="field/loading_area/2/color";
    subscription = m_client->subscribe(sub,0);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere2(QMqttMessage)));

    sub="field/loading_area/3/color";
    subscription = m_client->subscribe(sub,0);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere3(QMqttMessage)));

    sub="field/loading_area/4/color";
    subscription = m_client->subscribe(sub,0);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere4(QMqttMessage)));
}



/**reception de message du subs cam
 *
 * @brief Client::messReceivedCam
 * @param msg
 */
void Client::messReceivedCam(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("robot");
    QJsonValue response2 = rootObject.value("color");
    if(response2.toString()==colorballtargeted){
        colok=true;
    }else{
        colok=false;
    }
    qDebug()<<response;
    qDebug()<<response2;
    QJsonObject envoie;
    QString topic;
    int zoneDeChargement = receptz;
    QString zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";    // verifier le topic =========//
    quint8 qos;
    qos=0;
    bool retain;
    retain = true;
    envoie.insert("color",response2.toString());
    this->emissionJson(envoie,topic,qos,retain);
}

/**Message recu du robot indiquant son status
 * @brief Client::messReceivedRobStatus
 * @param msg
 */
void Client::messReceivedRobStatus(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("status");
    qDebug()<<response;
    id=jsonMessagePath(response.toInt());

}

/**Message recu du robot indiquant le status du boutton la stm32
 * @brief Client::messReceivedRobButton
 * @param msg
 */
void Client::messReceivedRobButton(QMqttMessage msg)
{

    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("package");


    qDebug()<<response;
    if(response.toBool()==true) //il y a quelque chose sur le robot
    {
        scan(); //appel du scan
        id++;
        qDebug()<<"text";
        qDebug()<<id;
        ballonbot = true; //il y a quelque chose sur le robot (utile pour l'algo)
        id=jsonMessagePath(id);
    }
    else
    {
        ballonbot = false;
        if(id!=12)
        {
            //ne rien faire
            id++;
            id=jsonMessagePath(id);

        }
        else //au dépot
        {
            qDebug()<<id;
            QJsonObject envoie;
            QString topic;
            quint8 qos;
            qos=2;
            bool retain;
            retain = false;
            id=0;
            topic = "field/robot/ROBOT5/path";
            envoie.insert("id",id);
            envoie.insert("direction", pazr.value(id));
            //envois ordre pour le prendre direction le premier noeud
            this->emissionJson(envoie,topic,qos,retain);

        }
    }
}

/**Message recu par etagere1
 * @brief Client::messReceiveEtagere1
 * @param msg
 */
void Client::messReceiveEtagere1(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    coletagere.insert(1,response.toString());
    qDebug()<<response;
    updatestatusetagere();
}

/**Message recu par etagere2
 * @brief Client::messReceiveEtagere2
 * @param msg
 */
void Client::messReceiveEtagere2(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    coletagere.insert(2,response.toString());
    qDebug()<<response;
    updatestatusetagere();
}

/**Message recu par etagere3
 * @brief Client::messReceiveEtagere3
 * @param msg
 */
void Client::messReceiveEtagere3(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    qDebug()<<response;
    coletagere.insert(3,response.toString());
    updatestatusetagere();
}

/**Message recu par etagere4
 * @brief Client::messReceiveEtagere4
 * @param msg
 */
void Client::messReceiveEtagere4(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    qDebug()<<response;
    coletagere.insert(4,response.toString());
    updatestatusetagere();
}

//*******************************************************************************************************************
//          AFFICHAGE ET BOUTONS                                                                                    *
//*******************************************************************************************************************

/**bouton de co/deco avec modification du bouton
 *
 * @brief Client::bp_co_clicked
 */
void Client::bp_co_clicked()
{
    m_client = new QMqttClient(this); //client
    m_client->setHostname(ui->le_host->text()); //hote
    m_client->setPort(ui->sb_port->value()); //port
    m_client->setUsername("terrain2"); //terrain
    m_client->setPassword("56jpwYhr"); //mdp
    //m_client->connectToHost(); //connection a l'hote

    //connect(m_client,SIGNAL(connected()),this,SLOT(setsub())); //active les subs
    qDebug() << m_client->state();
    if (m_client->state() == QMqttClient::Disconnected)
    {
        ui->le_host->setEnabled(false);
        ui->sb_port->setEnabled(false);
        ui->pb_co->setText(tr("Connecté"));
        ui->pb_co->setEnabled(false);
        m_client->connectToHost();
        qDebug() << "dasn le if" << m_client->state();
        m_client->setState(QMqttClient::Connecting);
        connect(m_client,SIGNAL(connected()),this,SLOT(setsub())); //active les subs
        ui->pb_order->setEnabled(true);

        //si client non co
    }
    /*
    else
    {
        ui->le_host->setEnabled(true);
        ui->sb_port->setEnabled(true);
        ui->pb_co->setText(tr("Connect"));
        m_client->disconnectFromHost();
        m_client->setState(QMqttClient::Disconnected);
        qDebug() << "dasn le else" << m_client->state();

    }
    */
}

/**mis a jour des couleurs sur l'affichage
 * @brief Client::updatestatusetagere
 */
void Client::updatestatusetagere()
{
    // couleur etagere 1
    if(coletagere.value(1)=="red")
    {
        ui->te_etage1->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0); }");
    }
    if(coletagere.value(1)=="green")
    {
        ui->te_etage1->setStyleSheet("QTextEdit { background-color: rgb(0, 255, 0); }");
    }
    if(coletagere.value(1)=="blue")
    {
        ui->te_etage1->setStyleSheet("QTextEdit { background-color: rgb(0, 0, 255); }");
    }
    if(coletagere.value(1)=="yellow")
    {
        ui->te_etage1->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 0); }");
    }

    // couleur etagere 2
    if(coletagere.value(2)=="red")
    {
        ui->te_etage2->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0); }");
    }
    if(coletagere.value(2)=="green")
    {
        ui->te_etage2->setStyleSheet("QTextEdit { background-color: rgb(0, 255, 0); }");
    }
    if(coletagere.value(2)=="blue")
    {
        ui->te_etage2->setStyleSheet("QTextEdit { background-color: rgb(0, 0, 255); }");
    }
    if(coletagere.value(2)=="yellow")
    {
        ui->te_etage2->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 0); }");
    }

    // couleur etagere 3
    if(coletagere.value(3)=="red")
    {
        ui->te_etage3->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0); }");
    }
    if(coletagere.value(3)=="green")
    {
        ui->te_etage3->setStyleSheet("QTextEdit { background-color: rgb(0, 255, 0); }");
    }
    if(coletagere.value(3)=="blue")
    {
        ui->te_etage3->setStyleSheet("QTextEdit { background-color: rgb(0, 0, 255); }");
    }
    if(coletagere.value(3)=="yellow")
    {
        ui->te_etage3->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 0); }");
    }

    // couleur etagere 4
    if(coletagere.value(4)=="red")
    {
        ui->te_etage4->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0); }");
    }
    if(coletagere.value(4)=="green")
    {
        ui->te_etage4->setStyleSheet("QTextEdit { background-color: rgb(0, 255, 0); }");
    }
    if(coletagere.value(4)=="blue")
    {
        ui->te_etage4->setStyleSheet("QTextEdit { background-color: rgb(0, 0, 255); }");
    }
    if(coletagere.value(4)=="yellow")
    {
        ui->te_etage4->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 0); }");
    }

    ui->label_3->setText(coletagere.value(1));
    ui->label_4->setText(coletagere.value(2));
    ui->label_7->setText(coletagere.value(3));
    ui->label_8->setText(coletagere.value(4));
}





