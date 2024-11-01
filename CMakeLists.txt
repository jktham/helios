cmake_minimum_required(VERSION 3.0.0)
project(helios)

set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE EXTERNAL_SOURCES external/src/*.cpp external/src/*.c)

add_executable(helios ${SOURCES} ${EXTERNAL_SOURCES})
target_include_directories(helios PRIVATE external/include)
if(WIN32)
	target_link_directories(helios PRIVATE external/lib/GLFW)
	target_link_libraries(helios PRIVATE libglfw3.a)
elseif(UNIX)
	target_link_libraries(helios PRIVATE glfw)
endif()
target_link_libraries(helios PRIVATE OpenGL::GL)

add_compile_definitions(GLFW_INCLUDE_NONE)
