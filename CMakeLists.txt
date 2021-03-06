cmake_minimum_required(VERSION 3.6)
project(triangle)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

configure_file(fragment.frag fragment.frag COPYONLY)
configure_file(vertex.vert vertex.vert COPYONLY)
configure_file(white.frag white.frag COPYONLY)
configure_file(white.vert white.vert COPYONLY)
configure_file(text.frag text.frag COPYONLY)
configure_file(text.vert text.vert COPYONLY)

# Define sources and executable
set(EXECUTABLE_NAME "triangle")
add_executable(${EXECUTABLE_NAME} main.cpp shader.hpp shader.cpp utility.hpp utility.cpp vbo.hpp vbo.cpp vao.hpp vao.cpp model.hpp triangle.hpp triangle.cpp engine.hpp engine.cpp camera.hpp camera.cpp loaded_model.hpp loaded_model.cpp mesh.hpp mesh.cpp lightsources.hpp lightsources.cpp object.hpp object.cpp text.hpp text.cpp)

add_custom_command(TARGET triangle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/nanosuit $<TARGET_FILE_DIR:triangle>/nanosuit)

add_custom_command(TARGET triangle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/cyborg $<TARGET_FILE_DIR:triangle>/cyborg)

add_custom_command(TARGET triangle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/planet $<TARGET_FILE_DIR:triangle>/planet)

add_custom_command(TARGET triangle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/rock $<TARGET_FILE_DIR:triangle>/rock)

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

find_package(ASSIMP REQUIRED)
if (ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${ASSIMP_LIBRARIES})
endif ()

find_package(SOIL REQUIRED)
if (SOIL_FOUND)
    include_directories(${SOIL_INCLUDE_PATH})
    target_link_libraries(${EXECUTABLE_NAME} ${SOIL_LIBRARY})
endif (SOIL_FOUND)

find_package(Freetype REQUIRED)
if (FREETYPE_FOUND)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES})
endif (FREETYPE_FOUND)