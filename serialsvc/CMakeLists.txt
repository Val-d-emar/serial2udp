project (serialsvc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets Network Test REQUIRED )
find_package(Qt$\{QT_VERSION_MAJOR\} COMPONENTS Core Gui Widgets Network Test REQUIRED )

set(HEADERS
    include/serialsvc.h
    )

set(SRC_LIST
    src/serialsvc.cpp
    )

add_library($\{PROJECT_NAME\} $\{SRC_LIST\} $\{HEADERS\})

target_include_directories( $\{PROJECT_NAME\}
    PUBLIC /include
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

target_link_libraries(serialsvc PRIVATE Qt$\{QT_VERSION_MAJOR}::Core Qt$\{QT_VERSION_MAJOR\}::Gui Qt$\{QT_VERSION_MAJOR\}::Widgets Qt$\{QT_VERSION_MAJOR\}::Network Qt$\{QT_VERSION_MAJOR\}::Test)



