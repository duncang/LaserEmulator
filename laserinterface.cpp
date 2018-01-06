#include "laserinterface.h"

#include <QtSerialPort/QSerialPort>
#include <QTime>
#include <iostream>

QT_USE_NAMESPACE

LaserInterface::LaserInterface(QObject *parent) :
    QThread(parent),
    m_waitTimeout(0),
    m_quit(false)
{

}


LaserInterface::~LaserInterface()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

QString LaserInterface::PortName()
{
    return m_portName;
}

void LaserInterface::setPortName(const QString &portName)
{
    this->m_portName = portName;
    emit PortNameChanged();
    return;
}

void LaserInterface::transaction(const QString &portName, int waitTimeout, const QString &request)
{   
    QMutexLocker locker (&m_mutex);
    this->m_portName = portName;
    this->m_waitTimeout = waitTimeout;
    this->m_request = request;

    if (!isRunning())
    {
        start();
    } else
    {
        m_cond.wakeOne();
    }

}


void LaserInterface::run()
{
    bool currentPortNameChanged = false;

    std::cout << "STARTING" << std::endl;

    m_mutex.lock();
    QString currentPortName;
    if (currentPortName != m_portName)
    {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentRequest = m_request;
    m_mutex.unlock();

    QSerialPort serial;

    if (currentPortName.isEmpty())
    {
        emit error(tr("Port name is empty!"));
        return;
    }

    while (!m_quit)
    {
        if (currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(9600);

            if (!serial.open(QIODevice::ReadWrite))
            {
                emit error(tr("Could not open port %!: %2").arg(m_portName).arg(serial.error()));
                return;

            } else
            {
                serial.clear();
            }
        }

        // write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);
        std::cout << "SENDING (" << requestData.toStdString() << ")" << std::endl;

        if (serial.waitForBytesWritten(m_waitTimeout))
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
                std::cout << "RECEIVED (" << response.toStdString() << ")" << std::endl;
                emit this->response(response);
            } else
            {
                emit timeout(tr("Wait read request timeout %1").arg(QTime::currentTime().toString()));
                std::cout << "READ TIMEOUT" << std::endl;
            }
        } else
        {
            emit timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
        }

        m_mutex.lock();
        m_cond.wait(&m_mutex);

        if (currentPortName != m_portName)
        {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        } else
        {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = m_waitTimeout;
        currentRequest = m_request;
        m_mutex.unlock();




    }

}
