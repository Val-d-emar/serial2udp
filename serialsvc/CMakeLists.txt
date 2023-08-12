project (serialsvc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED Network)

file(GLOB HEADERS
    include/*.h
    )

file(GLOB SRC_LIST
    src/*.cpp
    )

add_library(${PROJECT_NAME}
    ${SRC_LIST}
    ${HEADERS}
    )
target_include_directories( ${PROJECT_NAME}
    PUBLIC include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    )



