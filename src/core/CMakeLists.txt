add_library(core AdminClient.hpp
        KafkaConsumer.cpp KafkaConsumer.h
        ConsumerRecord.h
        Limiter.cpp Limiter.h
        Filter.cpp Filter.h
        ConsumerRecordBuffer.cpp ConsumerRecordBuffer.h
        KafkaProducer.cpp KafkaProducer.h
        ProducerRecord.h
        Types.h
        AbstractConverter.cpp AbstractConverter.h
        KafkaAdmin.cpp KafkaAdmin.h
        Error.cpp Error.h
        ConsumerRecordsExporter.cpp ConsumerRecordsExporter.h
        GroupInfo.cpp GroupInfo.h
)

target_link_libraries(core
        PRIVATE
        Qt6::Core
        Threads::Threads
        Rdkafka
        spdlog::spdlog
        spdlog::spdlog_header_only
        utils
)



