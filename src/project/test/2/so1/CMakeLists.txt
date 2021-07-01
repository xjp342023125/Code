PROJECT(xxso1)

SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -Wl,--exclude-libs,ALL")
#print compile info
set(CMAKE_VERBOSE_MAKEFILE ON)

#head file path
include_directories(${PROJECT_SOURCE_DIR}/include)


aux_source_directory(${PROJECT_SOURCE_DIR}/src/ SRC_FILES)
add_library(xxso1 SHARED ${SRC_FILES})

#so .a
target_link_libraries(xxso1
)