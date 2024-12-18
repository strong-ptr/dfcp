cmake_minimum_required(VERSION 3.10)

project(DFCP_Proj VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(external/cpr)
file(GLOB SRC_FILES src/*.cc)
add_executable(DFCP ${SRC_FILES})
target_link_libraries(DFCP PRIVATE cpr::cpr)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(DFCP PRIVATE -Wall -Wextra -pedantic)
endif()

