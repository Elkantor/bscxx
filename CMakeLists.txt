cmake_minimum_required (VERSION 3.9)

## Include libraries ##
include_directories ("${PROJECT_BINARY_DIR}/../include")
## End of include libraries ##

## Add executable for the app project ##
add_subdirectory(src)
## End of adding executables ##