PROJECT(xxlib)

#head file path
include_directories(./include)


aux_source_directory(./src/ SRC_FILES)
add_library(xxlib ${SRC_FILES})