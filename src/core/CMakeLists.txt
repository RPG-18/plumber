add_library(core AdminClient.hpp KafkaConsumer.cpp KafkaConsumer.h ConsumerRecord.h Limiter.cpp Limiter.h Filter.cpp Filter.h)

target_link_libraries(core
        Qt${QT_VERSION_MAJOR}::Core
        Threads::Threads
        Rdkafka
)



