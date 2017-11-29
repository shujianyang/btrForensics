include_directories(${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Tools)

add_executable(fsstat fsstat.cpp)
target_link_libraries(fsstat Pool Trees)
target_link_libraries(fsstat Trees Pool Basics Utility tsk)

add_executable(fls fls.cpp)
target_link_libraries(fls Pool Trees)
target_link_libraries(fls Trees Pool Basics Utility tsk)

add_executable(icat icat.cpp)
target_link_libraries(icat Pool Trees)
target_link_libraries(icat Trees Pool Basics Utility tsk)

add_executable(istat istat.cpp)
target_link_libraries(istat Pool Trees)
target_link_libraries(istat Trees Pool Basics Utility tsk)

add_executable(subls subls.cpp)
target_link_libraries(subls Pool Trees)
target_link_libraries(subls Trees Pool Basics Utility tsk)

add_executable(devls devls.cpp)
target_link_libraries(devls Pool Trees)
target_link_libraries(devls Trees Pool Basics Utility tsk)

