#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include "streamdecoder.h"
#include <thread>
#include <chrono>
#include <cmath>

#include "glwidget.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    d_startRecording(false),
    d_pauseRecording(false),
    d_stopRecording(false)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    eurlers = {0.0, 0.0, 0.0};

    ui->actionActionConnect->setEnabled(true);
    ui->actionActionDisconnect->setEnabled(false);
    ui->actionActionConfigure->setEnabled(true);
    ui->actionStartRecording->setEnabled(false);
    ui->actionPauseRecording->setEnabled(false);
    ui->actionStopRecording->setEnabled(false);


    status = new QLabel;
    ui->statusBar->addWidget(status);
    initActionsConnections();


    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
              this, &MainWindow::handleError);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);


        ui->chart_1->addGraph();
        ui->chart_2->addGraph();
        ui->chart_3->addGraph();


        QCPItemStraightLine *arrow0 = new QCPItemStraightLine(ui->chart_1);
        QCPItemStraightLine *arrow1 = new QCPItemStraightLine(ui->chart_2);
        QCPItemStraightLine *arrow2 = new QCPItemStraightLine(ui->chart_3);


        QCPItemStraightLine *arrow0_max = new QCPItemStraightLine(ui->chart_1);
        QCPItemStraightLine *arrow1_max = new QCPItemStraightLine(ui->chart_2);
        QCPItemStraightLine *arrow2_max = new QCPItemStraightLine(ui->chart_3);

        arrow0->setPen(QPen(Qt::green));
        arrow1->setPen(QPen(Qt::green));
        arrow2->setPen(QPen(Qt::green));

        arrow0->point1->setCoords(0, 0);
        arrow0->point2->setCoords(100, 0);
        arrow1->point1->setCoords(0, 0);
        arrow1->point2->setCoords(100, 0);
        arrow2->point1->setCoords(0, 0);
        arrow2->point2->setCoords(100, 0);

        arrow0_max->setPen(QPen(Qt::red));
        arrow1_max->setPen(QPen(Qt::red));
        arrow2_max->setPen(QPen(Qt::red));

        arrow0_max->point1->setCoords(0, 0);
        arrow0_max->point2->setCoords(100, 0);
        arrow1_max->point1->setCoords(0, 0);
        arrow1_max->point2->setCoords(100, 0);
        arrow2_max->point1->setCoords(0, 0);
        arrow2_max->point2->setCoords(100, 0);



        ui->chart_1->addItem(arrow0);
        ui->chart_2->addItem(arrow1);
        ui->chart_3->addItem(arrow2);

        ui->chart_1->yAxis->setRange(-190,190);
        ui->chart_2->yAxis->setRange(-190,190);
        ui->chart_3->yAxis->setRange(-190,190);




        ui->chart_1->addItem(arrow0_max);
        ui->chart_2->addItem(arrow1_max);
        ui->chart_3->addItem(arrow2_max);



        connect(ui->myGLWidget, SIGNAL(xRotationChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
        connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)), ui->horizontalSlider_2, SLOT(setValue(int)));
        connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)), ui->horizontalSlider_3, SLOT(setValue(int)));

        timer.start(12, this);
}


void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
        serial->setPortName(p.name);
        serial->setBaudRate(p.baudRate);
        serial->setDataBits(p.dataBits);
        serial->setParity(p.parity);
        serial->setStopBits(p.stopBits);
        serial->setFlowControl(p.flowControl);
        if (serial->open(QIODevice::ReadWrite)) {
          //  console->setEnabled(true);
          //  console->setLocalEchoEnabled(p.localEchoEnabled);
            ui->actionActionConnect->setEnabled(false);
            ui->actionActionDisconnect->setEnabled(true);
            ui->actionActionConfigure->setEnabled(false);
            ui->actionStartRecording->setEnabled(true);

            showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        } else {
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            showStatusMessage(tr("Open error"));
        }

}


void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
          serial->close();
     // console->setEnabled(false);
      ui->actionActionConnect->setEnabled(true);
      ui->actionActionDisconnect->setEnabled(false);
      ui->actionActionConfigure->setEnabled(true);
      showStatusMessage(tr("Disconnected"));

}

void MainWindow::writeData(const QByteArray &data)
{
 serial->write(data);
}


std::vector<std::string> MainWindow::explode(std::string const& s, char const & c)
{
    std::string buff{""};
    std::vector<std::string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    v.pop_back();
    return v;
}


float MainWindow::decodeFloat(std::string const &inString)
{

    static unsigned char H[256], L[256];
      if (L['1'] == 0)
      {
          for (int i=0; i<10; i++)
          {
              H['0'+i] = (i<<4);
              L['0'+i] = i;
          }
          for (int i=0; i<6; i++)
          {
              H['a'+i] = H['A'+i] = ((10+i)<<4);
              L['a'+i] = L['A'+i] = (10+i);
          }
      }
      std::vector<unsigned char> result(inString.size()>>1);
      for (int i=0,n=inString.size(); i<n-1; i+=2)
        result[i>>1] = H[inString[i]]+L[inString[i+1]];




    int intbits = (result[3] << 24) | ((result[2] & 0xff) << 16) | ((result[1] & 0xff) << 8) | (result[0] & 0xff);


     u.i = intbits;
     return u.f;
}



std::vector<float> MainWindow::quaternionToEuler(std::vector<float> const  &q)
{
    std::vector<float> euler(3);
    euler[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
    euler[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
    euler[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi

    return euler;
}

#include <string>

void MainWindow::readData()
{

    if(d_startRecording)
    {
       QByteArray data = serial->readLine();
       QString qstr = QString(data);

       if (qstr.size() == 38)
       {
      //   ui->plainTextEdit->appendPlainText(QString::number(qstr.length()) + '\n');

           streamDecoder.setStreamLine(data);
           streamDecoder.updateEurlers(eurlers);

           ui->plainTextEdit->appendPlainText("=================================\n");
           for (auto& it : eurlers)
           {
               QString str = QString::number(it);
               ui->plainTextEdit->appendPlainText(" q : " +  str);
           }
           ui->plainTextEdit->appendPlainText("=================================\n");

       }


      for (int i = 0; i < eurlers.size(); i++)
      {
          if (i == 0)
          {
              ui->chart_1->graph(0)->addData(counter, eurlers[0]);
              ui->chart_1->replot();
             // ui->chart_1->

              ui->chart_1->xAxis->rescale(true);


              counter++;

        //      if (counter % 10 == 0)

          }

          if (i == 1)
          {
              ui->chart_2->graph(0)->addData(counter1, eurlers[1]);
              ui->chart_2->replot();
            //  ui->chart_2->rescaleAxes(true);

              ui->chart_2->xAxis->rescale(true);


              counter1++;
          //    if (counter1 % 10 == 0)


          }


          if (i == 2)
          {
              ui->chart_3->graph(0)->addData(counter1, eurlers[2]);
              ui->chart_3->replot();
           //   ui->chart_3->rescaleAxes(true);

                       ui->chart_3->xAxis->rescale(true);

              counter2++;

           //   if (counter2 % 10 == 0)


          }
      }
    }

}


void MainWindow::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
/*    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
*/
    ui->myGLWidget->setYRotation(static_cast<int>(eurlers[0]));
    ui->myGLWidget->setXRotation(static_cast<int>(eurlers[1]));
    ui->myGLWidget->setZRotation(static_cast<int>(eurlers[2]));
    ui->myGLWidget->updateGL();

}


void MainWindow::about()
{

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
           QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
           closeSerialPort();
       }
}



void MainWindow::callFromThread()
{




}

 void MainWindow::pressRecordingButton(bool &button)
 {


 }

void MainWindow::startRecording()
{
    d_stopRecording = false;
    d_pauseRecording = false;
    d_startRecording = true;
    ui->actionStartRecording->setEnabled(false);
    ui->actionStopRecording->setEnabled(true);
    ui->actionPauseRecording->setEnabled(true);
}

void MainWindow::pauseRecording()
{
    d_stopRecording = false;
    d_pauseRecording = true;
    d_startRecording = false;
    ui->actionStartRecording->setEnabled(true);
    ui->actionStopRecording->setEnabled(true);
    ui->actionPauseRecording->setEnabled(false);
}

void MainWindow::stopRecording()
{
    d_stopRecording = true;
    d_pauseRecording = false;
    d_startRecording = false;
    ui->actionStartRecording->setEnabled(true);
    ui->actionStopRecording->setEnabled(false);
    ui->actionPauseRecording->setEnabled(true);
}


void MainWindow::initActionsConnections()
{
    connect(ui->actionActionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionActionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionActionConfigure, &QAction::triggered, settings, &MainWindow::show);
    connect(ui->actionStartRecording, &QAction::triggered, this, &MainWindow::startRecording);
    connect(ui->actionStopRecording, &QAction::triggered, this, &MainWindow::stopRecording);
    connect(ui->actionPauseRecording, &QAction::triggered, this, &MainWindow::pauseRecording);
}

void MainWindow::showStatusMessage(const QString &message)
{
status->setText(message);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}
