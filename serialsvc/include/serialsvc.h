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
    explicit SerialSvc(QObject *parent = nullptr);
    void openSerialPort(QString name);
    void closeSerialPort();
    int parceIn(QByteArray in);
    QStringList reads;
    bool isTimeOut();
    void startTime();
    uint port;
signals:
    void onCompleted(QString data);
public slots:
    void onTimer();
    void readStr();
    void doEncript(QString data);

protected:
    QTimer *timer;
    QUdpSocket *udpSocket;
    uint counter;
    uint maxtime;
    QByteArray buff;
};

int ser2udp(int argc, char** argv);

#endif

