cmake_minimum_required(VERSION 3.16)
PROJECT(xxbin)

#SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -Wl,--exclude-libs,ALL")

#head file path
include_directories(./include)
include_directories(../so1/include/)
include_directories(../so2/include/)

#so .a path
link_directories(../so1/build/)
link_directories(../so2/build/)

#bin
aux_source_directory(./src/ SRC_FILES)
add_executable(xxbin ${SRC_FILES})

#so .a
target_link_libraries(xxbin
xxso2
xxso1
)