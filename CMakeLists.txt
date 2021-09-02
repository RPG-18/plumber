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

if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    add_compile_options("-Wall" "-Werror" "-Wextra" "-Wshadow" "-Wno-unused-result")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

option(BUILD_OPTION_CLANG_TIDY "Build with clang-tidy enabled" OFF)
if (BUILD_OPTION_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

    if (CLANG_TIDY_EXE)
        message(STATUS "Use clang-tidy: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY clang-tidy -warnings-as-errors=* -header-filter='src/.*')
    else ()
        message(FATAL_ERROR "The clang-tidy executable not found!")
    endif ()

else ()
    message(STATUS "With NO clang-tidy build option")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include_directories(3rdparty/modern-cpp-kafka/include
        src/components
        src/configuration
        src/services
        src/core
        src
        )

set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools QuickControls2 Svg REQUIRED)
find_package(Librdkafka REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Boost REQUIRED)

set(TS_FILES src/kafkaui_ru_RU.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/qml.qrc
        src/qtquickcontrols2.conf
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
        Rdkafka
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickControls2
        Qt${QT_VERSION_MAJOR}::Svg
        spdlog::spdlog
        spdlog::spdlog_header_only
        Boost::boost
        core
        components
        configuration
        services
        utils)
