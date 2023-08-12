#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QString>
#include <QStringList>
#include "serialsvc.h"
#include "message.h"

TEST(TestParcing, BasicAssertions) {
  QByteArray in = "\n1234\n12345\n123456\n\n\n8\n";
  SerialSvc svc;
  EXPECT_EQ( svc.parceIn(in), 4);
  EXPECT_STREQ( svc.reads[0].toLocal8Bit(), "1234");
  EXPECT_STREQ( svc.reads[1].toLocal8Bit(), "12345");
  EXPECT_STREQ( svc.reads[2].toLocal8Bit(), "123456");
  EXPECT_STREQ( svc.reads[3].toLocal8Bit(), "8");
}

TEST(TestEncoding, BasicAssertions) {
  QString data = "12345678";
  QByteArray enc = Message(data).encrypt();
  EXPECT_STREQ( enc, "HELLO23456789");
}

TEST(TestBroadcasting, BasicAssertions) {
  int c = 1;
  char ** argv = new char*[1];
  QCoreApplication a(c, argv);
  SerialSvc svc(45454);
  QString data = "12345678";
  for (int i=0; i<=50; i++){
      emit svc.onCompleted(data);
      a.processEvents();
      if (svc.datagram.size() > 0) {
          break;
      }
      usleep(100000);
  }
  EXPECT_STREQ( svc.datagram, "HELLO23456789");
}

TEST(TestRun, BasicAssertions) {
  char ** argv = new char*[3];
  QString ar0("proga");
  QString ar1("/dev/serial0");
  QString ar2("64000");
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
  EXPECT_EQ( 10, 10);
  int argc = 1;
  EXPECT_EQ( ser2udp(argc, argv), 10);
  argc = 3;
  EXPECT_EQ( ser2udp(argc, argv), 20);
}
