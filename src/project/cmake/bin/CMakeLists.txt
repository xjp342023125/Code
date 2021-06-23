cmake_minimum_required(VERSION 3.16)
PROJECT(xxbin)

#head file path
include_directories(./include)
include_directories(../so/include/)
include_directories(../lib/include/)

#so .a path
link_directories(../so/build/)
link_directories(../lib/build/)

#bin
aux_source_directory(./src/ SRC_FILES)
add_executable(xxbin ${SRC_FILES})

#so .a
target_link_libraries(xxbin
xxso
xxlib
)