#ifndef LASERINTERFACE_H
#define LASERINTERFACE_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class LaserInterface : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString PortName READ PortName WRITE setPortName NOTIFY PortNameChanged)

public:
    explicit LaserInterface(QObject *parent = nullptr);
    ~LaserInterface();

    Q_INVOKABLE void transaction(const QString &portName, int waitTimeout, const QString &request);
    void run() Q_DECL_OVERRIDE;
    void setPortName(const QString &portName);
    QString PortName();

signals:
    void response (const QString &s);
    void error (const QString &s);
    void timeout (const QString &s);
    void PortNameChanged();

private:

    QString m_portName;
    int m_waitTimeout;
    QString m_request;

    QMutex m_mutex;
    QWaitCondition m_cond;

    bool m_quit;


};

#endif // LASERINTERFACE_H
