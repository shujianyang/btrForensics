include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. STRUCT_SRCS)

add_library(Structures ${STRUCT_SRCS})

#target_link_libraries(Structures Basics Utility)
