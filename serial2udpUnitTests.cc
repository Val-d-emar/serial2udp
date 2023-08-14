#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QFile>
#include <QString>
#include <QStringList>
#include "serialsvc.h"
#include "message.h"

TEST(TestParcing, BasicAssertions) {
  QByteArray in = "\n1234\n12345\n123456\n\n\n8\n";
  SerialSvc svc;
  ASSERT_EQ( svc.parceIn(in), 4);
  ASSERT_STREQ( svc.reads[0].toLocal8Bit(), "1234");
  ASSERT_STREQ( svc.reads[1].toLocal8Bit(), "12345");
  ASSERT_STREQ( svc.reads[2].toLocal8Bit(), "123456");
  ASSERT_STREQ( svc.reads[3].toLocal8Bit(), "8");
}

TEST(TestEncoding, BasicAssertions) {
  QString data = "12345678";
  QByteArray enc = Message(data).encrypt();
  ASSERT_STREQ( enc, "HELLO23456789");
}

TEST(TestBroadcasting, BasicAssertions) {
  int c = 1;
  char ** argv = new char*[1];
  QCoreApplication a(c, argv);
  SerialSvc svc(45454, "/dev/serial", &a);
  QString data = "12345678";
  emit svc.onCompleted(data);
  for (int i=0; i<=50; i++){
      a.processEvents();
      if (svc.datagram.size() > 0) {
          break;
      }
      usleep(100000);
  }
  ASSERT_STREQ( svc.datagram, "HELLO23456789");
}

TEST(TestErrors, BasicAssertions) {
  char ** argv = new char*[3];
  int argc = 1;
  ASSERT_EQ( ser2udp(argc, argv), 10);
  argc = 2;
  ASSERT_EQ( ser2udp(argc, argv), 10);
  QString ar0("proga");
  QString ar1("/dev/wrongSerialPort");
  QString ar2("wrongUdpPort");
  QByteArray bar = ar1.toLatin1();
  char *str=(char *)malloc(bar.size()+1);
  strncpy(str, bar.data(), bar.size());
  argv[1] = str;
  bar = ar2.toLatin1();
  str=(char *)malloc(bar.size()+1);
  strncpy(str, bar.data(), bar.size());
  argv[2] = str;
  bar = ar0.toLatin1();
  str=(char *)malloc(bar.size()+1);
  strncpy(str, bar.data(), bar.size());
  argv[0] = str;
  argc = 3;
  ASSERT_EQ( ser2udp(argc, argv), 20);
//  ar2 = "64000";
//  bar = ar2.toLatin1();
//  str=(char *)malloc(bar.size()+1);
//  strncpy(str, bar.data(), bar.size());
//  argv[2] = str;
//  EXPECT_EQ( ser2udp(argc, argv), 30);
}

TEST(TestComm, BasicAssertions) {
    // for pass this test you need using null-modem cross cable
    // or run socat loopback like me in Linux:
    //# socat -d -d pty,raw,echo=0 pty,raw,echo=0
    char ** argv = new char*[3];
    QString ar0("proga");
    QString ar1("/dev/serial");
    QString ar2("45454");
    QByteArray bar = ar1.toLatin1();
    char *str=(char *)malloc(bar.size()+1);
    strncpy(str, bar.data(), bar.size());
    argv[1] = str;
    bar = ar2.toLatin1();
    str=(char *)malloc(bar.size()+1);
    strncpy(str, bar.data(), bar.size());
    argv[2] = str;
    bar = ar0.toLatin1();
    str=(char *)malloc(bar.size()+1);
    strncpy(str, bar.data(), bar.size());
    argv[0] = str;
    int argc = 3;
    QCoreApplication a(argc, argv);

    QFile f(QCoreApplication::applicationDirPath()+"/socat.ls");
    QString lsCMD = QString("ls /dev/pts/* > \"%1\"").arg(f.fileName());
    std::system(lsCMD.toStdString().c_str());

    QStringList ls, ls2;

    if (f.exists()) {
        f.open(QIODevice::ReadOnly);
        QString load(f.readAll());
        ls.append(load.split(QLatin1Char('\n'), Qt::SkipEmptyParts));//
        f.close();
    }
    qDebug() << ls;
    QString CMD = QString("socat -d -d pty,raw,echo=0 pty,raw,echo=0 >/dev/null &");
    std::system(CMD.toStdString().c_str());
    std::system(QString("sleep 1").toStdString().c_str());
    std::system(lsCMD.toStdString().c_str());
    if (f.exists()) {
        f.open(QIODevice::ReadOnly);
        QString load(f.readAll());
        ls2.append(load.split(QLatin1Char('\n'), Qt::SkipEmptyParts));
        f.close();
        qDebug() << ls2;
        foreach (QString a, ls){
            if (ls2.contains(a)) ls2.removeAll(a);
        }
    }
    qDebug() << ls2;
    ASSERT_LE( ls2.size(), 2);

    qDebug() << QString(ls2[0]);
    qDebug() << QString(ls2[1]);

    QString outTTY(ls2[0]);
    QString inTTY(ls2[1]);
    QString testdata = "12345678";

    SerialSvc svc(ar2.toUInt(), inTTY, &a);
    CMD = QString("echo \"%1\" > %2")
            .arg(testdata)
            .arg(outTTY);
    for (int i=0; i<=6000; i++){
        std::system(CMD.toStdString().c_str());
        a.processEvents();
        if (svc.datagram.size() > 0) {
            break;
        }
        usleep(100000);
    }
    ASSERT_STREQ( svc.datagram, "HELLO23456789");
    std::system(QString("pkill socat").toStdString().c_str());
}
