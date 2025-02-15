cmake_minimum_required(VERSION 3.25)
project(useless-library LANGUAGES CXX)

option(BUILD_TESTS "Enable builds of tests" ON)

add_library(base INTERFACE
    src/distilled_set.hpp
)
add_library(useless::base ALIAS base)
target_compile_features(base INTERFACE cxx_std_20)
target_include_directories(base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(tests
        src/distilled_set.test.cpp
    )
    target_link_libraries(tests PRIVATE useless::base Catch2::Catch2WithMain)
endif()