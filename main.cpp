#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "laserinterface.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    qmlRegisterType<LaserInterface>("io.sensors.laser",1,0,"LaserInterface");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

//    LaserInterface laser;
//    laser.transaction("/dev/tty.usbserial",1000,"*00P1\r\n");

    return app.exec();
}
