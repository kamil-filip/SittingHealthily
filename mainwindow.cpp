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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    settings = new SettingsDialog;


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


    connect(ui->pushButton, SIGNAL (released()), this, SLOT (handleButton()));



        ui->chart_1->addGraph();
        ui->chart_2->addGraph();
        ui->chart_3->addGraph();




}


void MainWindow::updateChart1()
{

    ui->label->setText("Ala ma kta");
}

void MainWindow::updateChart2()
{


}


void MainWindow::handleButton()
{



}

void MainWindow::updateChart3()
{





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

    streamDecoder.setStreamLine(serial->readLine());

    //StreamDecoder ds(serial->readLine());
    std::vector<float> quaternions = streamDecoder.getQuaternions();

    ui->plainTextEdit->appendPlainText("=================================\n");
    for (auto& it : quaternions)
    {
        QString str = QString::number(it);
        ui->plainTextEdit->appendPlainText(" q : " +  str);
    }
    ui->plainTextEdit->appendPlainText("=================================\n");



  /*
    QByteArray data = serial->readLine();
    float q[4];

     std::vector<std::string> vecOfIn = explode(QString(data).toStdString(), ',');
     std::string str = std::to_string(vecOfIn.size());

     if (vecOfIn.size() == 4)
    {

      std::vector<float> quaternions;
      ui->plainTextEdit->appendPlainText("=================================\n");
      for (size_t idx = 0; idx < vecOfIn.size(); idx++)
      {
          QString temp = QString::fromStdString(vecOfIn[idx]);

          if (temp.length() == 8)
          {

              q[idx] = decodeFloat(vecOfIn[idx]);
              QString str = QString::number(q[idx]);
              ui->plainTextEdit->appendPlainText(" q" + QString::number(idx) + ": " +  str);
          }

      }
     ui->plainTextEdit->appendPlainText("=================================\n");

    }

    */




/*
    std::lock_guard<std::mutex> lock(myMutex);

    QByteArray data = serial->readLine();
    std::vector<std::string> vecOfIn = explode(QString(data).toStdString(), ',');
    std::vector<float> quaternions;
    float q[40];
*/

 /*


        for (size_t idx = 0; idx < vecOfIn.size(); idx++)
        {


              quaternions.push_back(decodeFloat(vecOfIn[idx]));

                /*q[idx] = decodeFloat(vecOfIn[idx]);
                  QString str = QString::number(q[idx]);

              ui->plainTextEdit->appendPlainText(" q" + QString::number(idx) + ": " +  str);*/


     //   }


/*
          std::vector<float> eurler = quaternionToEuler(quaternions);

          for (int i = 0; i < eurler.size(); i++)
          {
            //  QString str = QString::number(eurler[i] * 180 / 3.1415926);
             // ui->plainTextEdit->appendPlainText(" q" + QString::number(i) + ": " +  str);


              if (i == 0)
              {
                  ui->chart_1->graph(0)->addData(counter, eurler[0] * 180 / 3.1415926);
                  ui->chart_1->replot();
                  ui->chart_1->rescaleAxes(true);
                  counter++;

                  ui->openglWidget->setX(eurler[0] * 180 / 3.1415926);
              }

              if (i == 1)
              {
                  ui->chart_2->graph(0)->addData(counter1, eurler[1] * 180 / 3.1415926);
                  ui->chart_2->replot();
                  ui->chart_2->rescaleAxes(true);
                  counter1++;

                  ui->openglWidget->setY(eurler[1] * 180 / 3.1415926);
              }


              if (i == 2)
              {
                  ui->chart_3->graph(0)->addData(counter1, eurler[2] * 180 / 3.1415926);
                  ui->chart_3->replot();
                  ui->chart_3->rescaleAxes(true);
                  counter2++;

                  ui->openglWidget->setZ(eurler[2] * 180 / 3.1415926);

              }




          }
          */
 /*
    std::this_thread::sleep_for(std::chrono::milliseconds(100));



    QByteArray data = serial->readLine();

    DecodeStream ds(data);

    std::vector<float> floats = ds.getFloats();


    if (floats.size() > 3)
    {
        std::string output;



        for (auto&it : floats)
        {
            output += std::to_string(it) + " ";
        }

        output += '\n';
        ui->plainTextEdit->appendPlainText("===================================================\n");
        ui->plainTextEdit->appendPlainText(QString::fromStdString(output));
        ui->plainTextEdit->appendPlainText("===================================================\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    */

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

void MainWindow::startRecording()
{

    /* for (int i = 0; i < 100 ; i++)
     {
         ui->chart_1->graph(0)->addData(i, i);
         ui->chart_1->replot();
         std::this_thread::sleep_for (std::chrono::milliseconds(100));
         ui->chart_1->rescaleAxes(true);

         ui->chart_2->graph(0)->addData(i, i);
         ui->chart_2->replot();
         std::this_thread::sleep_for (std::chrono::milliseconds(100));
         ui->chart_2->rescaleAxes(true);


         ui->chart_3->graph(0)->addData(i, i);
         ui->chart_3->replot();
         std::this_thread::sleep_for (std::chrono::milliseconds(100));
         ui->chart_3->rescaleAxes(true);
     }
    */





}

void MainWindow::pauseRecording()
{

}

void MainWindow::stopRecording()
{

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
