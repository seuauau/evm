cmake_minimum_required(VERSION 3.10)

project(sequence C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wcast-align -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wno-long-long -Wcast-align -Winline -Werror -pedantic")

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

include_directories(${PROJECT_SOURCE_DIR}/inc)

set(APPNAME_TASK104 ${PROJECT_NAME}_testing_104)
set(APPNAME_TASK104_SELFTEST ${PROJECT_NAME}_selftest_104)

add_executable(${APPNAME_TASK104}          ${PROJECT_SOURCE_DIR}/src/main.c
                                           ${PROJECT_SOURCE_DIR}/src/task104.c)

add_executable(${APPNAME_TASK104_SELFTEST} ${PROJECT_SOURCE_DIR}/src/main.c
                                           ${PROJECT_SOURCE_DIR}/src/task104.c)

target_compile_definitions(${APPNAME_TASK104}          PRIVATE -DTASK104 -DTESTING_104)
target_compile_definitions(${APPNAME_TASK104_SELFTEST} PRIVATE -DTASK104 -DAUTOTESTING_104)

target_compile_options(${APPNAME_TASK104_SELFTEST} PRIVATE -g --coverage)

target_link_libraries(${APPNAME_TASK104}          PRIVATE m)
target_link_libraries(${APPNAME_TASK104_SELFTEST} PRIVATE m gcov)

add_custom_command(TARGET ${APPNAME_TASK104_SELFTEST} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/test/task104
            ${OUTPUT_DIR}/test/task104)
