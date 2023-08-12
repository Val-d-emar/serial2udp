#ifndef serialsvc_H
#define serialsvc_H

#include <QByteArray>
#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

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
//signals:
public slots:
    void onTimer();
    void readStr();

protected:
    QTimer *timer;
    uint counter;
    uint maxtime;
    QByteArray buff;
};

int ser2udp(int argc, char** argv);

#endif

