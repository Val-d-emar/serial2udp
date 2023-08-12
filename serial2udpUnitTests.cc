#include <gtest/gtest.h>
#include <QString>
#include <QStringList>
#include "serialsvc.h"
// Demonstrate some basic assertions.
TEST(Test, BasicAssertions) {
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

