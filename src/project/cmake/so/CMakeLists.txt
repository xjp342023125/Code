PROJECT(xxso)

SET(CMAKE_CXX_FLAGS "-fvisibility=hidden")
#print compile info
set(CMAKE_VERBOSE_MAKEFILE ON)

#head file path
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../lib/include/)

link_directories(${PROJECT_SOURCE_DIR}/../lib/build/)


aux_source_directory(${PROJECT_SOURCE_DIR}/src/ SRC_FILES)
add_library(xxso SHARED ${SRC_FILES})

#so .a
target_link_libraries(xxso
xxlib
)