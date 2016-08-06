#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <thread>
#include <chrono>

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


void MainWindow::readData()
{
    QByteArray data = serial->readLine();
    //console->putData(data);


    //ui->textBrowser->append(static_cast<QString>(data));

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


void MainWindow::startRecording()
{


    ui->chart_1->addGraph();
    ui->chart_2->addGraph();
    ui->chart_3->addGraph();

     for (int i = 0; i < 100 ; i++)
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
