#include <QDebug>
#include <QtGui/QGuiApplication>

#include "serialsvc.h"

int main(int argc, char** argv){

    QCoreApplication a(argc, argv);
    return ser2udp(argc, argv);
    return a.exec();
}
