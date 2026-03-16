cmake_minimum_required(VERSION 3.10)

project(EngineCore)

set(PROJECT_SOURCE_FILES 
    src/engine.cpp
    src/app.cpp
    src/input.cpp
    src/shader_program.cpp
    src/graphics_api.cpp
    src/material.cpp
    src/mesh.cpp
    src/render_queue.cpp
)

include_directories(src)

add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

add_subdirectory(third-party/glfw-3.4 "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")
include_directories(third-party/glfw-3.4/include)

set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
add_subdirectory(third-party/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/glew_build")

include_directories(third-party/glew/include)

set_property(TARGET glew_s PROPERTY FOLDER GLEW)
set_property(TARGET glew PROPERTY FOLDER GLEW)

target_link_libraries(${PROJECT_NAME}
    glfw
    glew_s
)