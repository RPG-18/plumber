if (UNIX)
    set(BRDKAFKA_LIB_SEARCH_PATH /usr/lib)
    set(RDKAFKA_INC_SEARCH_PATH /usr/include)
endif ()


find_path(RDKAFKA_INCLUDE_DIR librdkafka/rdkafka.h ${LIBRDKAFKA_INC_SEARCH_PATH})
find_library(RDKAFKA_LIBRARY rdkafka ${LIBRDKAFKA_LIB_SEARCH_PATH})

message(${RDKAFKA_INCLUDE_DIR} ${RDKAFKA_LIBRARY})

if(NOT TARGET Rdkafka)
    add_library(Rdkafka UNKNOWN IMPORTED)
    set_target_properties(Rdkafka PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${RDKAFKA_INCLUDE_DIR}")
    set_property(TARGET Rdkafka APPEND PROPERTY IMPORTED_LOCATION "${RDKAFKA_LIBRARY}")
endif()