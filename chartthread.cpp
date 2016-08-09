#include "chartthread.h"
#include <QtCore>


ChartThread::ChartThread(QObject *parent) : QThread(parent)
{

}

void ChartThread::run()
{
   /* for (int i = 0; i < 10000; i++)
    {
        QMutex mutex;
        mutex.lock();
        if(this->stop()) break;
        mutex.unlock();

        emit NumberChanged(i);
    }
    */
}
