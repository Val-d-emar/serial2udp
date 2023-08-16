cmake_minimum_required(VERSION 3.15)
project (serial2udp VERSION 1.0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -fno-delete-null-pointer-checks)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GTEST_DISABLE_PTHREADS OFF)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED Network SerialPort)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED Network SerialPort)

add_subdirectory(serialsvc)

file(GLOB SOURCES
    *.cpp
    )
file(GLOB HEADERS
    *.h
    )
file(GLOB QT_RESOURCES
    *.qrc
    )

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${QT_RESOURCES}
    ${HEADERS}
    )
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6Core_INCLUDE_DIRS} ${Qt6Quick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::SerialPort
    serialsvc    
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
  Qt${QT_VERSION_MAJOR}::SerialPort
  serialsvc
)

include(GoogleTest
    )
gtest_discover_tests(
    UnitTests
)
#add_executable(${PROJECT_NAME} serial2udpUnitTests.cc)

## ------------------ следующее - это упаковать содержимое пакета DEB с помощью cpack ------------------ ##
# Укажите номер версии пакета DEB
set(_VERSION_MAJOR 1)
set(_VERSION_MINOR 1)
set(_VERSION_PATCH 1)
set(CPACK_PACKAGE_VERSION_MAJOR "${_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${_VERSION_PATCH}")

# Укажите CPack, чтобы генерировать пакет DEB (или другие пакеты, такие как <.7z> <.zip>)
set(CPACK_GENERATOR "DEB")

# Укажите будущую компьютерную архитектуру
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
# Укажите имя пакета
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
# Укажите имя программы, имя после установки программы
set(CPACK_DEBIAN_PACKAGE_NAME "${PROJECT_NAME}")
# Укажите требуемую зависимость
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qtbase${QT_VERSION_MAJOR}-dev, qtbase${QT_VERSION_MAJOR}-private-dev, libqt${QT_VERSION_MAJOR}network${QT_VERSION_MAJOR}, libqt5serialport${QT_VERSION_MAJOR}-dev, libc6 (>= 2.34), libdlt2, libtinfo6")
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")
# Установите контактную информацию пакета
set(CPACK_PACKAGE_CONTACT "noreply@nomail.com")
# Установите человека обслуживания
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "members of nobody@nomail.com")
# Укажите файл, который необходимо установить
INSTALL(TARGETS ${PROJECT_NAME} UnitTests
        RUNTIME DESTINATION /opt/${PROJECT_NAME}
        )

include(CPack)
