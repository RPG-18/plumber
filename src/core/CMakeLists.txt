add_library(core AdminClient.hpp KafkaConsumer.cpp KafkaConsumer.h ConsumerRecord.h Limiter.cpp Limiter.h Filter.cpp Filter.h ConsumerRecordBuffer.cpp ConsumerRecordBuffer.h KafkaProducer.cpp KafkaProducer.h ProducerRecord.h Types.h)

target_link_libraries(core
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Threads::Threads
        Rdkafka
        spdlog::spdlog
        spdlog::spdlog_header_only
)



