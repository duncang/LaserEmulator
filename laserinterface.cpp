#include "laserinterface.h"

#include <QtSerialPort/QSerialPort>
#include <QTime>

QT_USE_NAMESPACE

LaserInterface::LaserInterface(QObject *parent) :
    QThread(parent),
    waitTimeout(0),
    quit(false)
{

}


LaserInterface::~LaserInterface()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void LaserInterface::transaction(const QString &portName, int waitTimeout, const QString &request)
{
    QMutexLocker locker (&mutex);
    this->portName = portName;
    this->waitTimeout = waitTimeout;
    this->request = request;

    if (!isRunning())
    {
        start();
    } else
    {
        cond.wakeOne();
    }

}


void LaserInterface::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    QString currentPortName;
    if (currentPortName != portName)
    {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRequest = request;
    mutex.unlock();

    QSerialPort serial;

    if (currentPortName.isEmpty())
    {
        emit error(tr("Port name is empty!"));
        return;
    }

    while (!quit)
    {
        if (currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite))
            {
                emit error(tr("Could not open port %!: %2").arg(portName).arg(serial.error()));
                return;

            }
        }

        // write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);

        if (serial.waitForBytesWritten(waitTimeout))
        {
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout))
            {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                {
                    responseData += serial.readAll();
                }

                QString response(responseData);
                emit this->response(response);
            } else
            {
                emit timeout(tr("Wait read request timeout %1").arg(QTime::currentTime().toString()));
            }
        } else
        {
            emit timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
        }

        mutex.lock();
        cond.wait(&mutex);

        if (currentPortName != portName)
        {
            currentPortName = portName;
            currentPortNameChanged = true;
        } else
        {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        currentRequest = request;
        mutex.unlock();




    }

}
