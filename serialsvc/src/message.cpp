#include "message.h"

Message::Message(QString inp, QObject *parent)
    :QObject(parent)
{
    input = inp.toLocal8Bit();
}

QByteArray Message::encrypt()
{
    QByteArray res = "HELLO";
    for (int i = 0 ; i< input.size(); i++){
        res.append(input[i]+1);
    }
    deleteLater();
    return res;
}
