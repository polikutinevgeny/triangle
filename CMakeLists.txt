cmake_minimum_required(VERSION 3.6)
project(triangle)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

configure_file(fragment.frag fragment.frag COPYONLY)
configure_file(vertex.vert vertex.vert COPYONLY)

# Define sources and executable
set(EXECUTABLE_NAME "triangle")
add_executable(${EXECUTABLE_NAME} main.cpp shader.hpp shader.cpp utility.hpp utility.cpp vbo.hpp vbo.cpp vao.hpp vao.cpp model.hpp triangle.hpp triangle.cpp engine.hpp engine.cpp camera.hpp camera.cpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
    # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
    target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
endif()