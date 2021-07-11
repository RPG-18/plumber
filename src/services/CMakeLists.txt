add_library(services STATIC
        ConfigurationService.cpp ConfigurationService.h
        Registry.cpp Registry.h
)

target_compile_definitions(services
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(services
        Qt${QT_VERSION_MAJOR}::Core
        spdlog::spdlog
)