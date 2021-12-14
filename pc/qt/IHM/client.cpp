#include "client.h"
#include "ui_client.h"
#include <QMqttClient>

QMap<int,QString> coletagere; //couleur dans les etageres

int id;     //
//chemin pour chaque cas
QMap<int,QString> pazr;
QMap<int,QString> pazc1;
QMap<int,QString> pazc2;
QMap<int,QString> pazc3;
QMap<int,QString> pazrd;
QMap<int,QString> pazrcr;
QMap<int,QString> etagerecolor;

QString colorballtargeted;

int receptz=0;
bool colok;
bool finishloop=false;
bool ballonbot;

void checketagere()
{
    bool ballfound=false;
    if(etagerecolor.value(1)==colorballtargeted){
            ballfound=true;
            receptz=1;
     }else if(etagerecolor.value(2)==colorballtargeted){
            ballfound=true;
            receptz=2;
     }else if(etagerecolor.value(3)==colorballtargeted){
        ballfound=true;
        receptz=3;
     }else if(etagerecolor.value(4)==colorballtargeted){
        ballfound=true;
        receptz=4;

    }
    if(ballfound==false){
        receptz=receptz%4;
        receptz++;
    }
}



Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

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
    pazc3.insert(5,"R");
    pazrd.insert(9,"F");
    pazrd.insert(10,"F");
    pazrd.insert(11,"F");
    pazrd.insert(12,"F");
    pazrcr.insert(9,"R");
    pazrcr.insert(1,"R");

    //initialise la co
    connect(this->ui->pb_co, SIGNAL(clicked()),this, SLOT(bp_co_clicked()));

    connect(this->ui->pushButton, SIGNAL(clicked()),this, SLOT(scan()));
   connect(this->ui->pb_order, SIGNAL(clicked()),this, SLOT(jsonMessageOrder()));
   coletagere.insert(1,"NoA");
   coletagere.insert(2,"NoA");
   coletagere.insert(3,"NoA");
   coletagere.insert(4,"NoA");
   updatestatusetagere();
   id=12;

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
    if (id<8){
       finishloop=false;
    }
        if(id<receptz){
            id++;
            pazr.value(id);
            //publish a et la value
            envoie.insert("id",id);
            envoie.insert("direction", pazr.value(id));

        }
        else if(id==receptz){

        }
        else if (id<9){
            if(id<4){
                id=4;
            }
            if(receptz==1){
                if (id==8){
                    finishloop=true;
                }else{
                    id++;
                    pazc1.value(id);
                    //publish a et la value
                    envoie.insert("id",id);
                    envoie.insert("direction", pazc1.value(id));
                }
            }
            if(receptz==2){
                if (id==8){
                    finishloop=true;
                }else{
                    id++;
                    pazc2.value(id);
                    //publish a et la value
                    envoie.insert("id",id);
                    envoie.insert("direction", pazc2.value(id));

                }
            }
            if(receptz==3){
                if (id==7){
                    finishloop=true;
                }else{
                    id++;
                    pazc2.value(id);
                    envoie.insert("id",id);
                    envoie.insert("direction", pazc2.value(id));

                }
            }
            if(receptz==4){
                if (id==5){
                    finishloop=true;
                }else{
                    id++;
                    pazc3.value(id);
                    //publish a et la value
                    envoie.insert("id",id);
                    envoie.insert("direction", pazc3.value(id));

                }
            }
        }
        if(finishloop==true){
            if(id<8){
                id=7;
            }
            if(colok==true){
                  id++;
                  qDebug()<<"testaa";
                  qDebug()<<id;
                  pazrd.value(id);
                  envoie.insert("id",id);
                  envoie.insert("direction", pazrd.value(id));

            }else{
                id++;
                qDebug()<<"testbb";
                if(id==10)
                    {
                    id=1;
                    //if(ballonbot==false){
                    //    checketagere();
                    //}
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
        if (id<13){
        this->emissionJson(envoie,topic,qos,retain);
        return id;
        }else{
            return 12;
        }

    /*
    QJsonObject envoie;
    QString topic;
    topic = "field/robot/ROBOT5/path";
    quint8 qos;
    qos=2;
    bool retain;
    retain = false;

    //payload: {“id”: <id_etape>, “direction”: <char_direction(L/F/R)>}
    envoie.insert("id","id_etape");
    envoie.insert("direction","char_dir");

    this->emissionJson(envoie,topic,qos,retain);
    */
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


/*
void Client::jsonMessageReception()
{


    qDebug().noquote() << "serveur me parle" << messageRecu;

    //transformation en jsondoc du Qbyte array (utf8)
    QJsonDocument messageJson = QJsonDocument::fromJson(messageRecu.toUtf8());
    //transformation en json object du jsondoc
    QJsonObject messageJsonObject = messageJson.object();
    jsonMessageReception(messageJsonObject);

}*/
/*
void Client::setSubCam(QMqttClient m_client)
{
    QString sub="field/camera/5/scan";
    QString subscription;
    subscription = m_client.subscribe(sub,1);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceived(QMqttMessage)));
}*/
void Client::scan()
{
    QString topic="field/camera/5/scan";
    QJsonObject checkObject;
    checkObject.insert("robot","ROBOT5");
    QJsonDocument jsDoc(checkObject);
    QString jsString = QString::fromLatin1(jsDoc.toJson());
    m_client->publish(topic, jsString.toUtf8(),2);
}
/**bouton de co/deco avec modification du bouton
 *
 * @brief Client::bp_co_clicked
 */
void Client::bp_co_clicked()
{
    m_client = new QMqttClient(this); //client
        m_client->setHostname(ui->le_host->text()); //hote
        m_client->setPort(ui->sb_port->value()); //port
        m_client->setUsername("terrain2");
        m_client->setPassword("56jpwYhr");
        m_client->connectToHost();
        connect(m_client,SIGNAL(connected()),this,SLOT(setsub()));
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

/**creation de sub
 * @brief Client::setsub
 */
void Client::setsub()
{
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



/**reception de message des subs
 *
 * @brief Client::messReceived
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
}

/**Message recu du robot
 * @brief Client::messReceivedRob
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

void Client::messReceivedRobButton(QMqttMessage msg)
{

    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("package");


    qDebug()<<response;
    if(response.toBool()==true)
    {
        scan();
        id++;
        qDebug()<<"text";
        qDebug()<<id;
        ballonbot = true;
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

        }else //au dépot
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
            //envois go premier noeud
            this->emissionJson(envoie,topic,qos,retain);

        }
    }
}

void Client::messReceiveEtagere1(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    coletagere.insert(1,response.toString());
    qDebug()<<response;
    updatestatusetagere();
}

void Client::messReceiveEtagere2(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    coletagere.insert(2,response.toString());
    qDebug()<<response;
    updatestatusetagere();
}

void Client::messReceiveEtagere3(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    qDebug()<<response;
    coletagere.insert(3,response.toString());
    updatestatusetagere();
}

void Client::messReceiveEtagere4(QMqttMessage msg)
{
    QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("color");
    qDebug()<<response;
    coletagere.insert(4,response.toString());
    updatestatusetagere();
}

void Client::updatestatusetagere()
{
    ui->label_3->setText(coletagere.value(1));
    ui->label_4->setText(coletagere.value(2));
    ui->label_7->setText(coletagere.value(3));
    ui->label_8->setText(coletagere.value(4));
}





