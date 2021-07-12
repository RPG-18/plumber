add_library(utils STATIC
        KafkaUtility.cpp KafkaUtility.h
)

target_link_libraries(utils
        PRIVATE
        spdlog::spdlog
        spdlog::spdlog_header_only
)