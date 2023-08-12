#include <QDebug>
#include <QtGui/QGuiApplication>

#include "serialsvc.h"

int main(int argc, char** argv){
    QCoreApplication a(argc, argv);
    int res = ser2udp(argc, argv);
    return res;
}
