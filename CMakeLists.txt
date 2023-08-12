cmake_minimum_required(VERSION 3.15)
project (serial2udp VERSION 1.0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED Network WebKitWidgets Test)
find_package(Qt$\{QT_VERSION_MAJOR\} COMPONENTS Core Gui Widgets REQUIRED Network WebKitWidgets Test)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_subdirectory(serialsvc)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG main
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(serial2udp main.cpp)

enable_testing()
add_executable(
  UnitTests
  serial2udpUnitTests.cc
)
target_link_libraries(
  UnitTests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(
    UnitTests
)
#add_executable(serial2udp serial2udpUnitTests.cc)

