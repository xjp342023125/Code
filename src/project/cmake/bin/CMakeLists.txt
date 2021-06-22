PROJECT(xxbin)

#head file path
include_directories(./include)


aux_source_directory(./src/ SRC_FILES)
add_executable(xxbin ${SRC_FILES})