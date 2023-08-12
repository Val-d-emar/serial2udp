#include <gtest/gtest.h>
#include <QString>
#include <QStringList>
#include "serialsvc.h"
// Demonstrate some basic assertions.


TEST(TestParcing, BasicAssertions) {
  // Expect two strings not to be equal.
  QByteArray in = "\n1234\n12345\n123456\n\n\n8\n";
  SerialSvc svc(nullptr);
  EXPECT_EQ( svc.parceIn(in), 4);
  EXPECT_STREQ( svc.reads[0].toLocal8Bit(), "1234");
  EXPECT_STREQ( svc.reads[1].toLocal8Bit(), "12345");
  EXPECT_STREQ( svc.reads[2].toLocal8Bit(), "123456");
  EXPECT_STREQ( svc.reads[3].toLocal8Bit(), "8");
}

TEST(TestRun, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  char ** argv = new char*[2];
//  QStringList args;
  QString ar("--test");
  QByteArray bar = ar.toLatin1();
  char *str=(char *)malloc(bar.size()+1);
  strncpy(str, bar.data(), bar.size());
  argv[1] = str;
  EXPECT_EQ( 10, 10);
  int argc = 2;
  EXPECT_EQ( ser2udp(argc, argv), 20);
  argc = 1;
  EXPECT_EQ( ser2udp(argc, argv), 10);
//  EXPECT_STREQ("test", "test");
}
