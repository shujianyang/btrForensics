include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. BASICS_SRCS)

add_library(Basics ${BASICS_SRCS})

#target_link_libraries(Structures Basics Utility)
