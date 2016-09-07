include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. STRUCT_SRCS)

add_library(Trees ${STRUCT_SRCS})

