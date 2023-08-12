#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include "serialsvc.h"

class Message : public QObject
{
    Q_OBJECT
public:
    Message(QString inp, QObject *parent = nullptr);
    QByteArray input;
    QByteArray encrypt();
signals:

};

#endif // MESSAGE_H
