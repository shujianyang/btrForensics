include_directories(${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Tools)

add_executable(fsstat fsstat.cpp)
target_link_libraries(fsstat Examiners Trees)
target_link_libraries(fsstat Trees Examiners Basics Utility tsk)

add_executable(fls fls.cpp)
target_link_libraries(fls Examiners Trees)
target_link_libraries(fls Trees Examiners Basics Utility tsk)

add_executable(icat icat.cpp)
target_link_libraries(icat Examiners Trees)
target_link_libraries(icat Trees Examiners Basics Utility tsk)

add_executable(istat istat.cpp)
target_link_libraries(istat Examiners Trees)
target_link_libraries(istat Trees Examiners Basics Utility tsk)

add_executable(subls subls.cpp)
target_link_libraries(subls Examiners Trees)
target_link_libraries(subls Trees Examiners Basics Utility tsk)
