#include "serialsvc.h"
#include <QDebug>
#include <QTimer>
#include <QtGui/QGuiApplication>

#include <QObject>
#include <QRegularExpression>
#include <QTimer>
#include "message.h"


void SerialSvc::readStr()
{
    QByteArray in = readAll();
    parceIn(in);
}

void SerialSvc::doEncript(QString data)
{
    QByteArray enc = Message(data).encrypt();
    udpSocket->writeDatagram(enc, QHostAddress::Broadcast, port);
}

int SerialSvc::parceIn(QByteArray in)
{
    int i = 0;
    if (in.contains(QLatin1Char('\n').toLatin1())){
        int n = in.indexOf('\n',0);
        while ( n >= 0) {
            if (n > 0)
                buff.append(in.left(n));
            if (buff.size()>0){
                reads.append(buff);
                emit onCompleted(reads.last());
                i++;
                buff.clear();
            }
            n = in.size()-n-1;
            if (n > 0)
                in = in.right(n);
            else break;
            n = in.indexOf('\n',0);
        }
    }else if (isTimeOut()){
        buff.append(in);
        if (buff.size() > 0){
            reads.append(buff);
            emit onCompleted(reads.last());
            i++;
        }
        buff.clear();
    } else {
        buff.append(in);
    }
    return i;
}

void SerialSvc::onTimer()
{
    if (++counter >= maxtime)
      timer->stop();
}

int ser2udp(int argc, char** argv){

    if ((argc>1) && 0 == qstrcmp(argv[1], "--test")){
        qDebug() << "if --test";
        return 20;
    }else{
         qDebug() << "else";
         QByteArray in = "\n1234\n12345\n123456\n\n\n8\n";
         SerialSvc svc(nullptr);
         qDebug() << svc.parceIn(in);
         qDebug() << svc.reads;
         return 10;
        }
}

void SerialSvc::closeSerialPort()
{
    if (isOpen())
        close();
    timer->stop();
}

bool SerialSvc::isTimeOut()
{
    if (++counter >= maxtime)
        return true;
    return false;
}

void SerialSvc::startTime()
{
    timer->stop();
    counter = 0;
    timer->start(1000);
}

SerialSvc::SerialSvc(QObject *parent)
    :QSerialPort(parent)
    ,port(64000)
    ,timer(new QTimer(parent))
    ,udpSocket(new QUdpSocket(parent))
    ,counter(0)
    ,maxtime(5)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect(this, SIGNAL(onCompleted(QString)), this, SLOT(doEncript(QString)));
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
