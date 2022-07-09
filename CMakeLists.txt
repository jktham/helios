cmake_minimum_required(VERSION 3.0.0)
project(helios)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE EXTERNAL_SOURCES external/src/*.cpp external/src/*.c)

add_executable(helios ${SOURCES} ${EXTERNAL_SOURCES})
target_include_directories(helios PRIVATE external/include)
target_link_directories(helios PRIVATE external/lib/GLFW)
target_link_libraries(helios PRIVATE libglfw3.a)
