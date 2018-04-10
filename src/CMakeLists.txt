project(app)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

if (MSVC)
    set(EXECUTABLE_OUTPUT_PATH bin/)
endif (MSVC)

file (GLOB_RECURSE source_files ./*)

add_executable (app ${source_files})