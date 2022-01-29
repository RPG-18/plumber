add_library(formats
        protobuf/ProtobufConverter.cpp protobuf/ProtobufConverter.h
        protobuf/ProtobufSourceTree.cpp protobuf/ProtobufSourceTree.h
        protobuf/ProtobufErrorCollector.cpp protobuf/ProtobufErrorCollector.h
        protobuf/ProtobufMessagesModel.cpp protobuf/ProtobufMessagesModel.h
        protobuf.qrc
        avro/AvroConverter.cpp avro/AvroConverter.h)

target_compile_definitions(formats
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(formats PRIVATE
        avrocpp
        Qt6::Core
        protobuf::libprotoc
        protobuf::libprotobuf
        protobuf::libprotobuf-lite
        spdlog::spdlog
        spdlog::spdlog_header_only
)