PROJECT(xxso2)

SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -Wl,--exclude-libs,ALL")
#print compile info
set(CMAKE_VERBOSE_MAKEFILE ON)

#head file path
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(../so2_ref/include/)

link_directories(${PROJECT_SOURCE_DIR}/../so2_ref/build/)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/ SRC_FILES)
add_library(xxso2 SHARED ${SRC_FILES})

#so .a
target_link_libraries(xxso2
xxso2_ref
)