PROJECT(xxso2_ref)

#SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -Wl,--exclude-libs,ALL")
#print compile info
set(CMAKE_VERBOSE_MAKEFILE ON)

#head file path
include_directories(${PROJECT_SOURCE_DIR}/include)


aux_source_directory(${PROJECT_SOURCE_DIR}/src/ SRC_FILES)
add_library(xxso2_ref SHARED ${SRC_FILES})

#so .a
target_link_libraries(xxso2_ref
)