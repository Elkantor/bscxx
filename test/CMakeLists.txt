project(bscxx_test)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 17)

include_directories ("${PROJECT_BINARY_DIR}/../../test/")

if (MSVC)
    set(EXECUTABLE_OUTPUT_PATH bin/)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif (MSVC)

file (GLOB_RECURSE test_files ./*)
file (GLOB_RECURSE test_source_files ../src/*)

FOREACH(item ${test_source_files})
  IF(${item} MATCHES "main.cc")
    LIST(REMOVE_ITEM test_source_files ${item})
  ENDIF(${item} MATCHES "main.cc")
ENDFOREACH(item)

add_executable (bscxx_test ${test_source_files} ${test_files})