include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. POOL_SRCS)

add_library(Pool ${POOL_SRCS})

