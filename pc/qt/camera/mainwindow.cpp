#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

using namespace std;

int counter=0;
int counter2=0;
 QRCodeDetector * qrDecoder = new QRCodeDetector();
 QString topic="field/camera/5/color";
 QString oldcol;
 QString col;
 int iLowH ;
 int iHighH ;
 int iLowS;
 int iHighS;
 int iLowV;
 int iHighV;
 QString namestored;
  void jaune(){  //paramètre pour la détection de la couleur jaune
        col="yellow";
        iLowH = 14;
        iHighH = 37;
        iLowS = 108;
        iHighS = 197;
        iLowV = 017;
        iHighV = 209;
  }
  void rouge(){ //paramètre pour la détection de la couleur rouge
      //pour le rouge
      col="red";
    /*  iLowH = 0;
      iHighH = 179;
      iLowS = 131;
      iHighS = 216;
      iLowV = 144;
      iHighV = 219;*/
      iLowH = 0;
      iHighH = 179;

      iLowS = 152;
      iHighS = 202;

      iLowV = 130;
      iHighV = 223;

  }
  void vert(){ //paramètre pour la détection de la couleur verte
        col="green";
        iLowH = 34;
        iHighH = 79;
        iLowS = 117;
        iHighS = 255;
        iLowV = 48;
        iHighV = 255;

  }
  void bleu(){ //paramètre pour la détection de la couleur bleu
      //pour le bleu
      col="blue";
      iLowH = 86;
      iHighH = 163;
      iLowS = 41;
      iHighS = 255;
      iLowV = 61;
      iHighV = 111;

  }
  void testColorFilter(Mat imgOriginal){  //fonction pour tester les filtres de couleurs pour déterminer la couleur de la balle
      Point center;
      counter2=0;
      Mat imgHSV;
      cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
      Mat imgThresholded;
      inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      //morphological opening (removes small objects from the foreground)
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      //morphological closing (removes small holes from the foreground)
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      cv::Canny(imgThresholded, imgThresholded, 100, 100, 3);
      vector<Vec3f> circles;
      HoughCircles(imgThresholded, circles, HOUGH_GRADIENT, 1,
                   imgThresholded.rows/16,  // change this value to detect circles with different distances to each other
                   220, 18, 50, 100 // change the last two parameters
              // (min_radius & max_radius) to detect larger circles
      );
      for( size_t i = 0; i < circles.size(); i++ )
      {

        counter2 ++;
         Vec3i c = circles[i];
          center = Point(c[0], c[1]);
          // circle center
          circle( imgOriginal, center, 1, Scalar(0,255,0), 3, LINE_AA);
          // circle outline
          int radius = c[2];
          circle(imgOriginal, center, radius, Scalar(0,255,0), 3, LINE_AA);

  }
  }
  QPixmap Mat2QPixmap(cv::Mat const& src)

  {

      cv::Mat temp(src.cols,src.rows,src.type()); // make the same cv::Mat

      cvtColor(src, temp,COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need

      QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);

      return QPixmap::fromImage(dest);

  }
MainWindow::MainWindow(QWidget *parent)//warning !!! nous rencontrons actuellement une erreur de crash avec aucune solution trouvé car le debuger ne donne aucune information, je n'ai pas de message d'erreur
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Camera controller AMAZÊN (by LQTS)®");
    m_client = new QMqttClient(this);
    QString name="mqtt-milles.imerir.org";
    m_client->setHostname(name);
    m_client->setPort(1884);
    m_client->setUsername("terrain2");
    m_client->setPassword("56jpwYhr");
    m_client->connectToHost();
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(setorder()));
    connect(m_client,SIGNAL(connected()),this,SLOT(setsub()));
    connect(m_client,SIGNAL(disconnected()),this,SLOT(dissub()));
    ui->label_5->setOpenExternalLinks(true);




}

MainWindow::~MainWindow()
{
    qDebug()<<"delete";
    delete ui;
}

void MainWindow::runCamera()//commande de lancement de camera
// le programme va d'abord chercher la présence du QRcode de notre robot, puis si un cercle est présent sur la photo, si c'est le cas , nous appliquons un filtre des differente couleurs pour reconnaitre celle qui correspond:
// de chaque couleur pour verifier la bonne couleur , il est a noté que nous a
{
    Point center;
   // open the first webcam plugged in the computer

   cv::VideoCapture camera(0);

  if (!camera.isOpened()) {
       std::cerr << "ERROR: Could not open camera" << std::endl;
   }

   // create a window to display the images from the webcam

  cv::namedWindow("Webcam");





   qrDecoder = new QRCodeDetector();
   std::vector<Point> points;
   counter=0;
   std::string data;
   vector<Vec3f> circles;
   Mat gray;
   cv::Mat frame;

  while (!namestored.isEmpty()) {
      counter=0;
      try {// nous effectuons un try and catch a cause d'une erreur de detection de QRcode du a la fonction qrcodedetector


           bool bSuccess = camera.read(frame); // read a new frame from video
           if (!bSuccess) //if not success, break loop
           {
               cout << "Cannot read a frame from video stream" << endl;
               break;
           }

               data = qrDecoder->detectAndDecode(frame,points);

               // if qrcode is found
               if (data.size() > 0)
               {
                   if (data==namestored.toStdString()){
                        cvtColor(frame, gray, COLOR_BGR2GRAY);
                        medianBlur(gray, gray, 5);


                        HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
                                     gray.rows/16,  // change this value to detect circles with different distances to each other
                                     220, 18, 50, 100 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                        );
                        for( size_t i = 0; i < circles.size(); i++ )
                        {
                            counter++;
                        }
                        if(counter>0){
                            jaune();
                             testColorFilter(frame);
                             if(counter2>0){
                                //balle jaune trouvé
                             }else{
                                 bleu();
                                  testColorFilter(frame);
                                  if(counter2>0){
                                    //balle bleu trouvé
                                  }else{
                                      rouge();
                                       testColorFilter(frame);
                                       if(counter2>0){
                                             // balle rouge trouvé
                                       }else{
                                           vert();
                                            testColorFilter(frame);
                                            if(counter2>0){
                                                    //balle verte trouvé
                                            }else{
                                                col="NO ball/colorunknown";

                                            }
                                       }
                                  }
                             }
                        }else{
                           //pas de balle detecté
                        }
                        camera.release();
                   }else{
                           //QRcode refusé
                   }
               }
               // show the image on the window
               //cv::imshow("Webcam", frame);



               ui->label_6->setPixmap(Mat2QPixmap(frame));
                if(counter>0){
                    if(m_client->Connected==2){
                    QJsonObject checkObject;
                    checkObject.insert("robot",namestored);
                    checkObject.insert("color",col);
                    QJsonDocument jsDoc(checkObject);
                    QString jsString = QString::fromLatin1(jsDoc.toJson());
                     m_client->publish(topic, jsString.toUtf8(),2);
                   }
                     namestored.clear();
             }

               // wait (10ms) for a key to be pressed
               if (cv::waitKey(10) >= 0){
                   break;
               }

   }  catch (cv::Exception error) {
        cout<<"retry code"<<endl;
        cout<<error.err<<endl;
        cout<<error.msg<<endl;
        delete qrDecoder;
        camera.release();
        qrDecoder = new QRCodeDetector();
        this->runCamera();


   }
   }
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceived(QMqttMessage)));
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(setorder()));

}

void MainWindow::messReceived(QMqttMessage msg)
{
 /*   QJsonDocument itemDoc = QJsonDocument::fromJson(msg.payload());
    QJsonObject rootObject = itemDoc.object();
    QJsonValue response = rootObject.value("robot");*/
    namestored="ROBOT5";
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceived(QMqttMessage)));
    disconnect(ui->pushButton,SIGNAL(clicked()),this,SLOT(setorder()));
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    this->runCamera();
}

void MainWindow::setsub()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->pushButton_2->setText("Deconnexion");
    ui->pushButton->setEnabled(true);

    QString sub="field/camera/5/scan";
    subscription = m_client->subscribe(sub,1);
    connect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceived(QMqttMessage)));
    disconnect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(cotohost()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(discotohost()));
}

void MainWindow::dissub()
{
    ui->lineEdit->setEnabled(true);
    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_4->setEnabled(true);
    ui->spinBox->setEnabled(true);
    ui->pushButton_2->setText("Connexion");
    ui->pushButton->setEnabled(false);
    QString sub="field/camera/5/scan";
    m_client->unsubscribe(sub);
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(cotohost()));
    disconnect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(discotohost()));
}

void MainWindow::setorder()
{
    namestored="ROBOT5";
    disconnect(ui->pushButton,SIGNAL(clicked()),this,SLOT(setorder()));
    disconnect(subscription,SIGNAL(messageReceived(QMqttMessage)),this,SLOT(messReceived(QMqttMessage)));
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    this->runCamera();
}

void MainWindow::cotohost()
{

    m_client->setHostname(ui->lineEdit->text());
    m_client->setPort(ui->spinBox->value());
    m_client->setUsername(ui->lineEdit_3->text());
    m_client->setPassword(ui->lineEdit_4->text());
    m_client->connectToHost();
}

void MainWindow::discotohost()
{
    m_client->disconnectFromHost();

}








