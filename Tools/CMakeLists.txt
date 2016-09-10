include_directories(${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Tools)

add_executable(fls fls.cpp)
target_link_libraries(fls Examiners Trees)
target_link_libraries(fls Trees Examiners Basics Utility tsk)

