cmake_minimum_required(VERSION 3.16)

project(Catch2Tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests test_glfw_setup.cpp test_camera.cpp)
target_sources(tests PRIVATE 
    ../src/glfw_setup.cpp
    ../src/camera.cpp
    $ENV{HOME}/OpenGL/src/glad.cpp
)
target_include_directories(tests PRIVATE 
    ../src/ 
    $ENV{HOME}/OpenGL/include
)
target_link_libraries(tests PRIVATE 
    Catch2::Catch2WithMain
    $ENV{HOME}/OpenGL/src/glfw3.lib
)

