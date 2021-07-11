add_library(configuration STATIC
        ClusterConfig.cpp
        ClusterConfig.h
        Configuration.cpp
        Configuration.h)

target_compile_definitions(configuration
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(configuration
        Qt${QT_VERSION_MAJOR}::Core
        spdlog::spdlog
)