include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. TOOLS_SRCS)

add_library(Tools ${TOOLS_SRCS})

