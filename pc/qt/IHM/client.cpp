//*******************************************************************************************************************
//          INCLUDE                                                                                              *
//*******************************************************************************************************************

#include "client.h"
#include "ui_client.h"
#include <QMqttClient>

//*******************************************************************************************************************
//          INITIALISATION                                                                                            *
//*******************************************************************************************************************

QMap<int,QString> coletagere; //couleur dans les etageres

int id;  //declaration de notre id d'etape

//chemin pour chaque cas
QMap<int,QString> pazr; //chemin zone
QMap<int,QString> pazc1;//chemin zone de chargement 1
QMap<int,QString> pazc2;// chemin zone de chargement 2
QMap<int,QString> pazc3;// chemin zone de chargement 3
QMap<int,QString> pazrd; // chemin retour depot
QMap<int,QString> pazrcr; // chemin retour zone de chargement precedent

QStringList listorder;//liste de nos ordres


QString colorballtargeted; //couleur de la balle ciblé

int receptz=0; // mise a 0 de la zone de recept
bool colok;    //bonne couleur
bool finishloop=false;// boucle non terminé (algo)
bool ballonbot; // balle sur le robot


//*******************************************************************************************************************
//          CONSTRUCTEUR                                                                                              *
//*******************************************************************************************************************

/**constructeur de client
 * @brief Client::Client
 * @param parent
 */
Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    this->setWindowTitle("AMAZËN (by LQTT) ®");
    this->setWindowIcon(QIcon("C:/Users/Laurent/Downloads/amazenpro.png"));
    //id de notre zone de depart --> zone de depot 1
    id=12;
    //mise en place des deplacement a effectuer en fonctions de l'id
    pazr.insert(0,"L");
    pazr.insert(1,"L");
    pazr.insert(2,"R");
    pazr.insert(3,"R");
    pazr.insert(4,"R");
    pazc1.insert(5,"L");
    pazc1.insert(6,"L");
    pazc1.insert(7,"L");
    pazc1.insert(8,"L");
    pazc2.insert(5,"L");
    pazc2.insert(6,"R");
    pazc2.insert(7,"L");
    pazc2.insert(8,"L");
    pazc3.insert(6,"R");
    pazrd.insert(9,"L");
    pazrd.insert(10,"L");
    pazrd.insert(11,"L");
    pazrd.insert(12,"L");
    pazrcr.insert(9,"R");
    pazrcr.insert(1,"R");

    //griser le bouton envoie ordre
    ui->pb_order->setEnabled(false);

    //lien
    ui->label_7->setOpenExternalLinks(true);

    //initialise la co
    connect(this->ui->pb_co, SIGNAL(clicked()),this, SLOT(bp_co_clicked()));

    //simulation du bouton de la stm
    connect(this->ui->pushButton, SIGNAL(clicked()),this, SLOT(scan()));

    //envoie de l'odre
    connect(this->ui->pb_order, SIGNAL(clicked()),this, SLOT(jsonMessageOrder()));

    // initialisation de l'affichage des étagères a Rien (blanc)
    coletagere.insert(1,"");
    coletagere.insert(2,"");
    coletagere.insert(3,"");
    coletagere.insert(4,"");

    //jeu avec la spinbox
    connect(ui->sb_zdc,SIGNAL(valueChanged(int)),this,SLOT(checkexspin(int)));

    //grise le bouton cam
    ui->pushButton->setEnabled(false);




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

    envoie.insert("color",ui->cb_color->currentText());      //couleur
    qDebug() << ui->cb_color->currentText();
    envoie.insert("loadingArea",ui->sb_zdc->value());   //zone de chargement
    envoie.insert("depositArea",1);                     //zone de dépot fixé
    envoie.insert("robotId","ROBOT5");                  // id_Robot

    this->emissionJson(envoie,topic,qos,retain);

    //si liste vide --> on recupere celle inscrite dans le ui
    if (listorder.isEmpty())
    {
        receptz = ui->sb_zdc->value();
        qDebug()<< "if de path et val de receptz "<<receptz;
        colorballtargeted = ui->cb_color->currentText();


    }
    //ajout des ordres dans la liste
    listorder.append(ui->sb_zdc->text()+","+ui->cb_color->currentText());
    ui->lcdNumber->display(listorder.size());
    ui->te_listeorder->append("Zone de Chargement: "+ui->sb_zdc->text()+" , couleur ciblée: "+ui->cb_color->currentText());

}


/**chemin a effectuer
 * Algo tres long prevoir aspirine
 * @brief Client::jsonMessagePath
 */
int Client::jsonMessagePath(int id)
{
    qDebug() << "valeur de receptz pour JSONMessagePath "<< receptz;
    int id2=id;

    //preparation de la trame
    QJsonObject envoie;
    QString topic;
    topic = "field/robot/ROBOT5/path";
    quint8 qos;
    qos=2;
    bool retain;
    retain = false;

    if (id<8) //cas ou la boucle du terrain n'est pas fini
    {
       finishloop=false;
    }


    if(id<receptz) //cas ou mon id est inferieur a ma zone de reception
    {
        id++;
        pazr.value(id);
        //publish id et sa value
        envoie.insert("id",id);
        envoie.insert("direction", pazr.value(id));

    }
    else if(id==receptz) //on est arrivé au bon endroit
    {
        //ne rien faire on est au bon endroit

    }
    //id qui ce situe soit en zone de reception soit interssection
    else if (id<9)
    {
        if(id<4)
        {
            id=4;
        }
        if(receptz==1)  //zone de recept 1
        {
            if (id==7)
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
        if(receptz==2) //zone de recept 2
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
        if(receptz==3) //zone de recept 3
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
        if(receptz==4)  //zone de recept 4
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
                id=8;//avantmodif id=7
        }
        if(colok==true)
        {
            id++;
            qDebug()<<"boucle true et color true ";
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
                    checketagere(); //cherche la couleur dans les étageres
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
        qDebug()<<"zzzz";
        if(id2!=receptz){
            qDebug()<<"ggg";
            this->emissionJson(envoie,topic,qos,retain); //envoie
        }
        return id;
    }
    else
    {
        return 12; //zone de départ
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

/**check couleur des etageres
 * @brief Client::checketagere
 */
void Client::checketagere()
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


//*******************************************************************************************************************
//          PARTIE SUB                                                                                              *
//*******************************************************************************************************************

/**creation des sub
 * @brief Client::setsub
 */
void Client::setsub()
{
    //envoie des color present dans les etageres
    QJsonObject envoie;
    QString topic;

    //zone de chargement 1
    int zoneDeChargement = 1;
    QString zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";
    quint8 qos;
    qos=0;
    bool retain;
    retain = true;
    envoie.insert("color","");
    this->emissionJson(envoie,topic,qos,retain);

    //zone de chargement 2
    zoneDeChargement = 2;
    zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";
    qos=0;
    retain = true;
    envoie.insert("color","");
    this->emissionJson(envoie,topic,qos,retain);

    //zone de chargement 3
    zoneDeChargement = 3;
    zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";
    qos=0;
    retain = true;
    envoie.insert("color","");
    this->emissionJson(envoie,topic,qos,retain);

    //zone de chargement 4
    zoneDeChargement = 4;
    zdc = QString::number(zoneDeChargement);
    topic = "field/loading_area/"+ zdc +"/color";
    qos=0;
    retain = true;
    envoie.insert("color","");
    this->emissionJson(envoie,topic,qos,retain);

    // abonnements camera pour couleur
    QString sub="field/camera/5/color";
    subscription = m_client->subscribe(sub,1);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedCam(QMqttMessage)));

    // abonnements ROBOT5n pour status
    sub="field/robot/ROBOT5/status";
    subscription = m_client->subscribe(sub,1);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedRobStatus(QMqttMessage)));

    // abonnements ROBOT5n pour button
    sub="field/robot/ROBOT5/button";
    subscription = m_client->subscribe(sub,2);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedRobButton(QMqttMessage)));

    // abonnements pour connaitre couleur etagere 1
    sub="field/loading_area/1/color";
    subscription = m_client->subscribe(sub,0);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere1(QMqttMessage)));

    // abonnements pour connaitre couleur etagere 2
    sub="field/loading_area/2/color";
    subscription = m_client->subscribe(sub,0);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere2(QMqttMessage)));

    // abonnements pour connaitre couleur etagere 3
    sub="field/loading_area/3/color";
    subscription = m_client->subscribe(sub,0);
    qDebug()<<subscription;
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere3(QMqttMessage)));

    // abonnements pour connaitre couleur etagere 4
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
    topic = "field/loading_area/"+ zdc +"/color";
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

    id=jsonMessagePath(response.toInt()); // recuperation de la valeur de l'id du chemin

    if (id==12)
    {
         listorder.removeFirst();
         ui->lcdNumber->display(listorder.size());

         //suppression du premiere ordre du QTextEdit te_listeorder
         QTextCursor cursor = ui->te_listeorder->textCursor();
         cursor.movePosition(QTextCursor::Start);
         cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 0);
         cursor.select(QTextCursor::LineUnderCursor);
         cursor.removeSelectedText();
         cursor.deleteChar(); //suppression de l'element

        if(!listorder.isEmpty())
        {
            receptz=listorder.at(0).split(",")[0][0].digitValue();
            colorballtargeted=listorder.at(0).split(",")[1][0];
            qDebug()<<id;
        }
    }
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
        if(id!=12){
            scan(); //appel du scan
            id++;
            qDebug()<<"text";
            qDebug()<<id;
            ballonbot = true; //il y a quelque chose sur le robot (utile pour l'algo)
            id=jsonMessagePath(id);
        }
    }
    else
    {
        ballonbot = false;
        colok=false;
        if(id!=12)
        {
            //ne rien faire
            id++;
            id=jsonMessagePath(id);

        }
        else //au dépot
        {
            if(!listorder.isEmpty()){
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
}

/**Message recu par etagere1 et mis a jour de la couleur etagere
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

/**Message recu par etagere2 et mis a jour de la couleur etagere
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

/**Message recu par etagere3 et mis a jour de la couleur etagere
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

/**Message recu par etagere4 et mis a jour de la couleur etagere
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
        ui->pb_co->setText(tr("Déconnexion"));
        ui->pushButton->setEnabled(true);
        m_client->connectToHost();
        qDebug() << "dasn le if" << m_client->state();
        m_client->setState(QMqttClient::Connecting);
        disconnect(m_client,SIGNAL(disconnected()),this,SLOT(dissub())); //active les subs
        connect(m_client,SIGNAL(connected()),this,SLOT(setsub())); //active les subs
        disconnect(this->ui->pb_co, SIGNAL(clicked()),this, SLOT(bp_co_clicked()));
        connect(this->ui->pb_co, SIGNAL(clicked()),this,SLOT(setdiscon())); //active les subs
        ui->pb_order->setEnabled(true);

    }
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
}

/**pour le bouton disconnect
 * @brief Client::setdiscon
 */
void Client::setdiscon()
{
    ui->le_host->setEnabled(true);
    ui->sb_port->setEnabled(true);
    ui->pb_co->setText(tr("Connexion"));
    ui->pb_order->setEnabled(false);
    m_client->disconnectFromHost();
    ui->te_listeorder->clear();
    listorder.clear();
    ui->lcdNumber->display(0);
    ui->pushButton->setEnabled(false);

    // reset de l'affichage des étagères a Rien (blanc)
    coletagere.insert(1,"");
    coletagere.insert(2,"");
    coletagere.insert(3,"");
    coletagere.insert(4,"");

    connect(this->ui->pb_co, SIGNAL(clicked()),this, SLOT(bp_co_clicked()));
    disconnect(this->ui->pb_co, SIGNAL(clicked()),this,SLOT(setdiscon())); //active les subs
    disconnect(m_client,SIGNAL(connected()),this,SLOT(setsub())); //active les subs
    connect(m_client,SIGNAL(disconnected()),this,SLOT(dissub())); //active les subs
}

/**quand deconnexion clicked() desinscription
 * @brief Client::dissub
 */
void Client::dissub()
{
    QString sub="field/camera/5/color";
    m_client->unsubscribe(sub);
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedCam(QMqttMessage)));

    sub="field/robot/ROBOT5/status";
    m_client->unsubscribe(sub);
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedRobStatus(QMqttMessage)));

    sub="field/robot/ROBOT5/button";
    m_client->unsubscribe(sub);
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceivedRobButton(QMqttMessage)));

    sub="field/loading_area/1/color";
    m_client->unsubscribe(sub);
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere1(QMqttMessage)));

    sub="field/loading_area/2/color";
    m_client->unsubscribe(sub);
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere2(QMqttMessage)));

    sub="field/loading_area/3/color";
    m_client->unsubscribe(sub);
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere3(QMqttMessage)));

    sub="field/loading_area/4/color";
    m_client->unsubscribe(sub);
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceiveEtagere4(QMqttMessage)));
}

/** check spin pour changer comboBox
 * @brief Client::checkexspin
 * @param spinindex
 */
void Client::checkexspin(int spinindex)
{
    disconnect(ui->cb_color,SIGNAL(currentIndexChanged(int)),this,SLOT(checkexcombo(int)));

    QString getval;
    int newint;
    getval=coletagere.value(spinindex);
    qDebug()<<getval;

    if (getval!=""){
       newint= ui->cb_color->findText(getval);

       ui->cb_color->setCurrentIndex(newint);
    }
    connect(ui->cb_color,SIGNAL(currentIndexChanged(int)),this,SLOT(checkexcombo(int)));

}
