#include "serialsvc.h"
#include <QDebug>
#include <QtGui/QGuiApplication>

int ser2udp(int argc, char** argv){

    if ((argc>1) && 0 == qstrcmp(argv[1], "--test")){
        qDebug() << "if --test";
        return 20;
    }else{
         qDebug() << "else";
         return 10;
        }
}
