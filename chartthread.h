#ifndef CHARTTHREAD_H
#define CHARTTHREAD_H

#include <QThread>


class ChartThread : public QThread
{
      Q_OBJECT
public:
    explicit ChartThread(QObject * parent = 0);
    void run();
    bool stop();

signals:
    void NumberChanged(int number);


};

#endif // CHARTTHREAD_H
