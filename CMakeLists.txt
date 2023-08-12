cmake_minimum_required(VERSION 3.15)
project (serial2udp VERSION 1.0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED Network SerialPort)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED Network SerialPort)

add_subdirectory(serialsvc)

#file(GLOB SOURCES
#    *.cpp
#)
#file(GLOB HEADERS
#    *.h
#    )
#file(GLOB QT_RESOURCES
#    *.qrc
#    )

set(SOURCES
    main.cpp
)
set(HEADERS
    main.h
    )
#set(QT_RESOURCES
#    res.qrc
#    )

add_executable(${PROJECT_NAME} ${SOURCES} ${QT_RESOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6Core_INCLUDE_DIRS} ${Qt6Quick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    serialsvc
    Qt${QT_VERSION_MAJOR}::SerialPort
    )

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG main
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(
  UnitTests
  serial2udpUnitTests.cc
)
target_link_libraries(
  UnitTests
  GTest::gtest_main
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  serialsvc
)

include(GoogleTest
    )
gtest_discover_tests(
    UnitTests
)
#add_executable(${PROJECT_NAME} serial2udpUnitTests.cc)

