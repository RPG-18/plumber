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
        Message.cpp Message.h)

target_compile_definitions(components
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(components
        Qt${QT_VERSION_MAJOR}::Core
        Threads::Threads
        Rdkafka
        core
)
