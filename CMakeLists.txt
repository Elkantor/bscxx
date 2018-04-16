cmake_minimum_required (VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

## Include libraries ##
include_directories ("${PROJECT_BINARY_DIR}/../include")
## End of include libraries ##

## Add executable for the app project ##
add_subdirectory(src)
## End of adding executables ##