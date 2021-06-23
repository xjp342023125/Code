PROJECT(xxso)

SET(CMAKE_CXX_FLAGS "-fvisibility=hidden")
#print compile info
set(CMAKE_VERBOSE_MAKEFILE ON)

#head file path
include_directories(./include)


aux_source_directory(./src/ SRC_FILES)
add_library(xxso SHARED ${SRC_FILES})

