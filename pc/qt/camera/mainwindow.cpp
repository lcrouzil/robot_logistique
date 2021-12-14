#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/objdetect.hpp>

#include <opencv2/imgcodecs.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include<QString>
using namespace cv;

using namespace std;

int counter=0;
 QString topic="qtmqtt/topic1";
 QString oldcol;
QString col;
  int iLowH ;
  int iHighH ;
  int iLowS;
  int iHighS;
  int iLowV;
  int iHighV;

  void jaune(){
        col="yellow";
        iLowH = 14;
        iHighH = 37;

        iLowS = 108;
        iHighS = 197;

        iLowV = 017;
        iHighV = 209;
  }
  void rouge(){
      //pour le rouge
      col="red";
      iLowH = 0;
      iHighH = 179;

      iLowS = 131;
      iHighS = 216;

      iLowV = 144;
      iHighV = 219;

  }
  void vert(){
        col="green";
        iLowH = 34;
        iHighH = 79;

        iLowS = 117;
        iHighS = 255;

        iLowV = 48;
        iHighV = 255;

  }
  void bleu(){
      //pour le bleu
      col="blue";
      iLowH = 86;
      iHighH = 163;

      iLowS = 41;
      iHighS = 255;

      iLowV = 61;
      iHighV = 111;

  }
  void testfilter(Mat imgOriginal){
      Point center;
      counter=0;
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

      imshow("Thresholded Image", imgThresholded); //show the thresholded image
      cv::Canny(imgThresholded, imgThresholded, 100, 100, 3);
      cv::imshow("3 canny",imgThresholded);


      vector<Vec3f> circles;
      HoughCircles(imgThresholded, circles, HOUGH_GRADIENT, 1,
                   imgThresholded.rows/16,  // change this value to detect circles with different distances to each other
                   220, 18, 0, 140 // change the last two parameters
              // (min_radius & max_radius) to detect larger circles
      );
      for( size_t i = 0; i < circles.size(); i++ )
      {

        counter ++;
         Vec3i c = circles[i];
          center = Point(c[0], c[1]);
          if (col=="v"){
              if(imgOriginal.at<Vec3b>(center)[2]<100){
                  // circle center
                  circle( imgOriginal, center, 1, Scalar(0,255,0), 3, LINE_AA);
                  // circle outline
                  int radius = c[2];
                  circle(imgOriginal, center, radius, Scalar(0,255,0), 3, LINE_AA);

              }

          }

          else if(col=="j"){
              if(imgOriginal.at<Vec3b>(center)[2]>100){
                  if(imgOriginal.at<Vec3b>(center)[1]>100){
                      // circle center
                      circle( imgOriginal, center, 1, Scalar(0,255,0), 3, LINE_AA);
                      // circle outline
                      int radius = c[2];
                      circle(imgOriginal, center, radius, Scalar(0,255,0), 3, LINE_AA);

                  }

              }
          }
          else if (col=="r"){
              if(imgOriginal.at<Vec3b>(center)[1]<100){
                      // circle center
                      circle( imgOriginal, center, 1, Scalar(0,255,0), 3, LINE_AA);
                      // circle outline
                      int radius = c[2];
                      circle(imgOriginal, center, radius, Scalar(0,255,0), 3, LINE_AA);

              }
          }else{
          // circle center
          circle( imgOriginal, center, 1, Scalar(0,255,0), 3, LINE_AA);
          // circle outline
          int radius = c[2];
          circle(imgOriginal, center, radius, Scalar(0,255,0), 3, LINE_AA);


          }
  }
  }
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_client = new QMqttClient(this);
    QString name="10.3.2.7";
    m_client->setHostname(name);
    m_client->setPort(20000);
    m_client->connectToHost();
    this->maine();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::maine()
{
    Point center;
   // open the first webcam plugged in the computer

   cv::VideoCapture camera(4);

   if (!camera.isOpened()) {

       std::cerr << "ERROR: Could not open camera" << std::endl;



   }


   // create a window to display the images from the webcam

   cv::namedWindow("Webcam");


   // this will contain the image from the webcam

   cv::Mat frame;

   QRCodeDetector qrDecoder =  QRCodeDetector();



   // display the frame until you press a key

   while (1) {

       // capture the next frame from the webcam

       camera >> frame;

       // detect and decode qrcode

       std::string data = qrDecoder.detectAndDecode(frame);

       // if qrcode is found

       if (data.size() > 0)

       {

          // cout << data << endl;
           if (data=="ROBOT5"){
               counter=0;
                Mat gray;
                cvtColor(frame, gray, COLOR_BGR2GRAY);
                medianBlur(gray, gray, 5);

                vector<Vec3f> circles;
                HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
                             gray.rows/16,  // change this value to detect circles with different distances to each other
                             220, 18, 0, 140 // change the last two parameters
                        // (min_radius & max_radius) to detect larger circles
                );
                for( size_t i = 0; i < circles.size(); i++ )
                {
                    counter++;
                    /*Vec3i c = circles[i];
                    Point center = Point(c[0], c[1]);
                    // circle center
                    circle( frame, center, 1, Scalar(0,255,0), 3, LINE_AA);
                    // circle outline
                    int radius = c[2];
                    circle( frame, center, radius, Scalar(0,255,0), 3, LINE_AA);*/
                }
               // imshow("gray", gray);
                if(counter>0){
                    jaune();
                     testfilter(frame);
                     if(counter>0){
                          topic="qtmqtt/topic1";


                     }else{
                         bleu();
                          testfilter(frame);
                          if(counter>0){

                            // cout << col+" circle" << endl;
                          }else{
                              rouge();
                               testfilter(frame);
                               if(counter>0){

                                 // cout << col+" circle" << endl;
                               }else{
                                   vert();
                                    testfilter(frame);
                                    if(counter>0){

                                       //cout << col+" circle" << endl;
                                    }else{
                                        col="0";


                                        //cout << "color unknown" << endl;
                                    }
                               }
                          }
                     }


                }else{
                    // cout << "no circle" << endl;
                }

           }else{
               //cout << "not granted" << endl;
           }

       }
       if (oldcol!=col && col!="0"){
            m_client->publish(topic, col.toUtf8());
            oldcol=col;
       }

       // show the image on the window

       cv::imshow("Webcam", frame);

       // wait (10ms) for a key to be pressed


       if (cv::waitKey(10) >= 0)
           break;

   }

}








