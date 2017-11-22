#ifndef LASERINTERFACE_H
#define LASERINTERFACE_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class LaserInterface : public QThread
{
    Q_OBJECT

public:
    explicit LaserInterface(QObject *parent = nullptr);
    ~LaserInterface();

    void transaction(const QString &portName, int waitTimeout, const QString &request);
    void run() Q_DECL_OVERRIDE;

signals:
    void response (const QString &s);
    void error (const QString &s);
    void timeout (const QString &s);

private:

    QString portName;
    int waitTimeout;
    QString request;

    QMutex mutex;
    QWaitCondition cond;

    bool quit;


};

#endif // LASERINTERFACE_H
