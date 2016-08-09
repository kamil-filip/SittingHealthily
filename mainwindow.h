#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <mutex>
#include "streamdecoder.h"


QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE


class Console;
class SettingsDialog;


class MainWindow : public QMainWindow
{
    Q_OBJECT


    union {
        float f;  // assuming 32-bit IEEE 754 single-precision
        int i;    // assuming 32-bit 2's complement int
     } u;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    std::vector<std::string> explode(std::string const& s, char const & c);
    float decodeFloat(std::string const &inString);

    long long counter = 0;
    long long counter1 = 0;
    long long counter2 = 0;

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);



    void updateChart1();
    void updateChart2();
    void updateChart3();

    void handleButton();

    void startRecording();
    void pauseRecording();
    void stopRecording();





private:
    void initActionsConnections();

private:
    void callFromThread();
    void showStatusMessage(const QString &message);
    float decodeFloat(QString inString);
    StreamDecoder streamDecoder;


    std::vector<float> quaternionToEuler(std::vector<float> const  &q);
    std::mutex myMutex;


    Ui::MainWindow *ui;
    QLabel *status;
    SettingsDialog *settings;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
