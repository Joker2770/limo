cmake_minimum_required(VERSION 3.10)
# Project name
project(limo)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
add_definitions (${GTK3_CFLAGS_OTHER})

set(CMAKE_C_FLAGS "-g -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "-g -static-libgcc -static-libstdc++")

set(IncludePath ${CMAKE_CURRENT_SOURCE_DIR}/)
set(SrcPath "${CMAKE_CURRENT_SOURCE_DIR}/")

# Sources
include_directories(${IncludePath})
include_directories (${GTK3_INCLUDE_DIRS})

#link_directories (${GTK3_LIBRARY_DIRS})

file(GLOB SOURCES_gtkUI "${SrcPath}/*.c")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES_gtkUI})
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
