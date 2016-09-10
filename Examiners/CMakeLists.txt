include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. EXAMINERS_SRCS)

add_library(Examiners ${EXAMINERS_SRCS})

