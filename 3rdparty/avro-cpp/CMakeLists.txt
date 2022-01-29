find_package(Snappy)

add_definitions(-DSNAPPY_CODEC_AVAILABLE)

include_directories(avro)
set(AVRO_SOURCE_FILES
        impl/Compiler.cc impl/Node.cc impl/LogicalType.cc
        impl/NodeImpl.cc impl/ResolverSchema.cc impl/Schema.cc
        impl/Types.cc impl/ValidSchema.cc impl/Zigzag.cc
        impl/BinaryEncoder.cc impl/BinaryDecoder.cc
        impl/Stream.cc impl/FileStream.cc
        impl/Generic.cc impl/GenericDatum.cc
        impl/DataFile.cc
        impl/parsing/Symbol.cc
        impl/parsing/ValidatingCodec.cc
        impl/parsing/JsonCodec.cc
        impl/parsing/ResolvingDecoder.cc
        impl/json/JsonIO.cc
        impl/json/JsonDom.cc
        impl/Resolver.cc impl/Validator.cc
        )

set_property(SOURCE ${AVRO_SOURCE_FILES} PROPERTY SKIP_AUTOGEN ON)
add_library(avrocpp STATIC ${AVRO_SOURCE_FILES})
target_link_libraries(avrocpp PRIVATE
        Boost::boost
        Boost::filesystem
        Boost::iostreams
        Boost::system
        Snappy::snappy
)
target_include_directories(avrocpp PRIVATE ${SNAPPY_INCLUDE_DIR})
set_property (TARGET avrocpp
        APPEND PROPERTY COMPILE_DEFINITIONS AVRO_SOURCE)