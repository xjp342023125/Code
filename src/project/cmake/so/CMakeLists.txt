PROJECT(xxso)

#head file path
include_directories(./include)


aux_source_directory(./src/ SRC_FILES)
add_library(xxso SHARED ${SRC_FILES})