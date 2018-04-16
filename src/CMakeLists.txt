project(bscxx)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(EXECUTABLE_OUTPUT_PATH bin/)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif (MSVC)

file (GLOB_RECURSE source_files ./*)

add_executable (bscxx ${source_files})