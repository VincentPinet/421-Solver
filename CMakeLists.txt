cmake_minimum_required(VERSION 3.13)
project(421-Solver VERSION 0.1.0 LANGUAGES CXX)

find_package(fmt CONFIG REQUIRED)

add_executable(main 421.cpp)
set_target_properties(main PROPERTIES CXX_STANDARD 17)
target_link_libraries(main PRIVATE fmt::fmt)

if(MSVC)
  target_compile_options(main PRIVATE /Wall /W4 /WX)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(main PRIVATE /O2)
  endif()
else()
  target_compile_options(main PRIVATE -O3 -Wall -Wextra -Wpedantic -Werror)
endif()
