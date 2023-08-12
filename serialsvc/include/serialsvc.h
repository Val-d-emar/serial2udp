#ifndef serialsvc_H
#define serialsvc_H

#include <QByteArray>
#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QUdpSocket>

class SerialSvc : public QSerialPort
{
    Q_OBJECT
public:
    void openSerialPort(QString TTYname );
    void closeSerialPort();
    int parceIn(QByteArray in);
    QStringList reads;
    bool isTimeOut();
    void startTime();
    uint portUDP;
    QByteArray datagram;
    SerialSvc(uint UDPport = 64000, QString TTYname = "/dev/serial", QObject *parent = nullptr);
signals:
    void onCompleted(QString data);
public slots:
    void onTimer();
    void readStr();
    void doEncript(QString data);
    void doReadUDP();

protected:
    QTimer *timer;
    QUdpSocket *udpSocket;
    uint counter;
    uint maxtime;
    QByteArray buff;
};

int ser2udp(int argc, char** argv);

#endif

