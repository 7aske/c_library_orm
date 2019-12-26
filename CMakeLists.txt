cmake_minimum_required(VERSION 3.15)
project(it350_pz_app C)
set(CMAKE_C_STANDARD 11)
set(TARGET it350_pz_app)

include_directories(internal include)
add_subdirectory(src)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
add_executable(${TARGET} main.c ${SOURCES})

execute_process(COMMAND mysql_config --cflags
        OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
        OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

target_compile_options(${TARGET} PUBLIC ${MYSQL_CFLAGS})
target_link_libraries(${TARGET} ${MYSQL_LIBS})