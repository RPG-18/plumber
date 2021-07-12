cmake_minimum_required(VERSION 3.14)

project(kafkaui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include_directories(3rdparty/modern-cpp-kafka/include
        src/components
        src/configuration
        src/services
        src/core
        src
        )

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools QuickControls2 REQUIRED)
find_package(Librdkafka REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(TS_FILES src/kafkaui_ru_RU.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/qml.qrc
        ${TS_FILES})

add_subdirectory(src)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kafkaui
            ${PROJECT_SOURCES}
            )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(kafkaui
            ${PROJECT_SOURCES}
            )
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_compile_definitions(kafkaui
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(kafkaui
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickControls2
        spdlog::spdlog
        spdlog::spdlog_header_only
        Rdkafka
        core
        components
        configuration
        services
        utils)
