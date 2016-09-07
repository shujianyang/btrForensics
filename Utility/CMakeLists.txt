include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. UTIL_SRCS)

add_library(Utility ${UTIL_SRCS})
