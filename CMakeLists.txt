cmake_minimum_required(VERSION 3.14)

project(plumber LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_OSX_DEPLOYMENT_TARGET "12")

include(cmake/tools.cmake)
if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    add_compile_options("-Wall" "-Wextra" "-Wshadow" "-Wno-unused-result")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS )
endif ()

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    add_definitions(-DRAPIDJSON_SSE42)
endif()

clang_tidy(-header-filter='${CMAKE_SOURCE_DIR}/src/*')
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include_directories(3rdparty/modern-cpp-kafka/include
        3rdparty/avro-cpp
        3rdparty/avro-cpp/avro
        3rdparty/rapidjson-1.1.0/include
        src/components
        src/configuration
        src/services
        src/core
        src/formats
        src
        )

set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)

find_package(Qt6 COMPONENTS Core Quick LinguistTools QuickControls2 Svg Widgets REQUIRED)
find_package(Librdkafka REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Boost 1.70.0 REQUIRED COMPONENTS filesystem iostreams regex system)
find_package(Protobuf REQUIRED)
find_package(Snappy CONFIG REQUIRED)

set(TS_FILES src/plumber_ru_RU.ts)

set(MACOSX_BUNDLE_ICON_FILE plumber.icns)
set(MACOSX_BUNDLE_GUI_IDENTIFIER com.github.RPG18.plumber)
set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/osx/rc/plumber.icns)
set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

set(PROJECT_SOURCES
        src/main.cpp
        src/qml.qrc
        src/qtquickcontrols2.conf
        win/rc/plumber.rc
        ${TS_FILES}
        ${APP_ICON_MACOSX})

add_subdirectory(3rdparty/avro-cpp)
add_subdirectory(src)

qt_add_executable(plumber
        WIN32
        MACOSX_BUNDLE
        ${PROJECT_SOURCES}
        )
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

set_target_properties(plumber PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/osx/rc/Info.plist.in")

target_compile_definitions(plumber
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(plumber
        PRIVATE
        Rdkafka
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Svg
        Qt6::Widgets
        spdlog::spdlog
        spdlog::spdlog_header_only
        Boost::boost
        core
        components
        configuration
        services
        utils
        formats
)
