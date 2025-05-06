cmake_minimum_required(VERSION 3.25)

# had to go with a french name for this one
project(solveur-quatre-cent-vingt-et-un VERSION 0.2.0 LANGUAGES CXX)

add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_26)
target_compile_options(main PRIVATE
    -Wall
    -Wcast-align
    -Wconversion
    -Wdouble-promotion
    -Wduplicated-branches
    -Wduplicated-cond
    -Werror
    -Wextra
    -Wformat=2
    -Wimplicit-fallthrough
    -Wlogical-op
    -Wmisleading-indentation
    -Wno-coverage-invalid-line-number
    -Wnon-virtual-dtor
    -Wnull-dereference
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpedantic
    -Wshadow
    # -Wsign-conversion # this one is dumb sometimes
    -Wsuggest-override
    -Wunused
    -Wuseless-cast
)
