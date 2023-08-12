#ifndef serialsvc_H
#define serialsvc_H

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
    QStringList reads;
    bool isTimeOut();
    void startTime();
//signals:

protected:
    QTimer timer;
    uint counter;
    uint maxtime;


protected slots:
    void readStr();
    void onTimer();
};

int ser2udp(int argc, char** argv);

#endif

