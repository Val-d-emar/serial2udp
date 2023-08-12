#include "serialsvc.h"
#include <QDebug>
#include <QTimer>
#include <QtGui/QGuiApplication>

#include <QObject>


void SerialSvc::readStr()
{
    const QByteArray data = readAll();

}

void SerialSvc::onTimer()
{
    if (++counter >= maxtime)
      timer.stop();
}

int ser2udp(int argc, char** argv){

    if ((argc>1) && 0 == qstrcmp(argv[1], "--test")){
        qDebug() << "if --test";
        return 20;
    }else{
         qDebug() << "else";
         return 10;
        }
}

void SerialSvc::closeSerialPort()
{
    if (isOpen())
        close();
    timer.stop();
}

bool SerialSvc::isTimeOut()
{
    if (++counter >= maxtime)
        return true;
    return false;
}

void SerialSvc::startTime()
{
    timer.stop();
    counter = 0;
    timer.start(1000);
}

SerialSvc::SerialSvc(QObject *parent)
    :timer(new QTimer(parent))
    , maxtime(5)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

void SerialSvc::openSerialPort(QString name)

{
    closeSerialPort();
    setPortName(name);
    reads.clear();
//    setBaudRate(baudRate);
//    setDataBits(dataBits);
//    setParity(parity);
//    setStopBits(stopBits);
//    setFlowControl(flowControl);
    if (open(QIODevice::ReadOnly)) {
        qDebug() << tr("Connected to serial ") << portName();
        startTime();
        connect(this, SIGNAL(readyRead()), this, SLOT(readStr()));
    } else {
        qDebug() << tr("Error open serial ") << errorString();
    }

}
