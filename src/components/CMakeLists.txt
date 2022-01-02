add_library(components
        KafkaConnectivityTester.h
        KafkaConnectivityTester.cpp
        ConfigModel.cpp ConfigModel.h
        Cluster.cpp Cluster.h
        BrokerModel.cpp BrokerModel.h
        TopicModel.cpp TopicModel.h
        Consumer.cpp Consumer.h
        ConsumerHelperModels.h ConsumerHelperModels.cpp
        MessageModel.cpp MessageModel.h
        Types.h Types.cpp
        Message.cpp Message.h
        TopicCreator.cpp TopicCreator.h
        ErrorWrap.cpp ErrorWrap.h
        Producer.cpp Producer.h
        ProtoOption.cpp ProtoOption.h
        Helpers.cpp Helpers.h
        Settings.cpp Settings.h)

target_compile_definitions(components
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(components
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Threads::Threads
        Rdkafka
        core
        spdlog::spdlog
        spdlog::spdlog_header_only
)

