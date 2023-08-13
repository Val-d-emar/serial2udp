#include "serialsvc.h"
#include <QDebug>
#include <QTimer>
#include <QtGui/QGuiApplication>

#include <QObject>
#include <QRegularExpression>
#include <QTimer>
#include <signal.h>
#include "message.h"
#include <unistd.h>


void SerialSvc::readStr()
{
    QByteArray in = readAll();
    startTime();
    parceIn(in);
    qDebug() << "Received serial data:" << in;
}

void SerialSvc::doEncript(QString data)
{
    QByteArray enc = Message(data).encrypt();
    udpSocket->writeDatagram(enc, QHostAddress::Broadcast, portUDP);
    qDebug() << "Send broadcast:" << enc << "port" << portUDP;
}

void SerialSvc::doReadUDP()
{
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Received datagram:" << datagram.constData();
    }
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
    if (++counter >= maxtime){
      timer->stop();
//      emit readyRead();
      parceIn(QByteArray());
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

SerialSvc::SerialSvc(uint UDPport, QString TTYname, QObject *parent)
    :QSerialPort(parent)
    ,errorOpens(false)
    ,timer(new QTimer(parent))
    ,udpSocket(new QUdpSocket(parent))
    ,counter(0)
    ,maxtime(5)

{
    portUDP = UDPport;
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect(this, SIGNAL(onCompleted(QString)), this, SLOT(doEncript(QString)));
    connect(this, SIGNAL(onCompleted(QString)), this, SLOT(startTime()));
    errorOpens = ! udpSocket->bind(portUDP, QUdpSocket::ShareAddress);
    if (!errorOpens)
        connect(udpSocket, SIGNAL(readyRead()), this,SLOT(doReadUDP()));
    else
        qDebug() << "Error bind UDP port:" << UDPport;
    errorOpens = errorOpens or !openSerialPort(TTYname);
}

SerialSvc::~SerialSvc()
{
    closePorts();
}

void SerialSvc::closePorts()
{
    disconnect();
    closeSerialPort();
    udpSocket->disconnectFromHost();
    udpSocket->close();
}

bool SerialSvc::openSerialPort(QString TTYname)
{
    closeSerialPort();
    setPortName(TTYname);
    reads.clear();
    //    setBaudRate(baudRate);
    //    setDataBits(dataBits);
    //    setParity(parity);
    //    setStopBits(stopBits);
    //    setFlowControl(flowControl);
    if (open(QIODevice::ReadOnly)) {
        qDebug() << tr("Connected to serial") << portName();
        startTime();
        connect(this, SIGNAL(readyRead()), this, SLOT(readStr()));
        return true;
    } else {
        qDebug() << tr("Error open serial") << errorString();       
    }
    return false;
}

SerialSvc *pserial;
bool closeApp = false;
void sigHunter(int sig)
{
    signal(sig, SIG_IGN);
    pserial->closePorts();
    pserial->deleteLater();
    closeApp = true;
}

int ser2udp(int argc, char** argv){
    QCoreApplication a(argc, argv);
    if ((argc>2)){
        QString portTTY(a.arguments().at(1));
        QString portUDP(a.arguments().at(2));

        bool ok = false;
        uint pUDP = portUDP.toUInt(&ok);
        if (ok){
           pserial = new SerialSvc(pUDP, portTTY, &a);
           if (pserial->errorOpens){
               pserial->closePorts();
               pserial->deleteLater();
               a.exit();
               qDebug() << "Halted";
               return 30;
           }
           // catch ctrl-c to shutdown cleanly
           signal(SIGINT, sigHunter);
           signal(SIGHUP, sigHunter);
           signal(SIGTERM, sigHunter);
           signal(SIGKILL, sigHunter);
           for (int i=0; i<=6000; i++){
               a.processEvents();
               if (closeApp
                   || pserial->reads.size() >= 10) {
                   usleep(300000);
                   break;
               }
               usleep(100000);
           }

        } else {
            qDebug() << "Usage:";
            qDebug() << "      " << a.applicationName().toStdString().c_str() << "/dev/serialX udpPort" ;
            a.exit();
            return 20;
        }
    }else{
         qDebug() << "Usage:";
         qDebug() << "      " << a.applicationName().toStdString().c_str() << "/dev/serialX udpPort" ;
         a.exit();
         return 10;
        }
    a.exit();
    return 0;
}
