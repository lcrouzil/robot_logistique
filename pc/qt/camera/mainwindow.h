#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>
#include <QJsonArray>
#include <QJsonValue>
#include<QJsonDocument>
#include<QJsonObject>
#include <opencv2/objdetect.hpp>

#include <opencv2/imgcodecs.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <QMqttSubscription>
#include <iostream>
#include<QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void runCamera();
public slots:
    void messReceived(QMqttMessage);
    void setsub();
    void dissub();
    void setorder();
    void cotohost();
    void discotohost();
private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
    QMqttSubscription * subscription;

};
#endif // MAINWINDOW_H
