cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(EngineCore)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

configure_file(
    ${PROJECT_SOURCE_DIR}/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
)

set(PROJECT_SOURCE_FILES 
    src/engine.cpp
    src/app.cpp
    src/input.cpp
    src/shader_program.cpp
    src/graphics_api.cpp
    src/material.cpp
    src/mesh.cpp
    src/render_queue.cpp
    src/scene.cpp
    src/game_object.cpp
    src/component.cpp
    src/mesh_component.cpp
    src/camera_component.cpp
    src/player_controller_component.cpp
    src/fs.cpp
)

include_directories(src)

add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

add_subdirectory(third-party/glfw-3.4 "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")
include_directories(third-party/glfw-3.4/include)

set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
add_subdirectory(third-party/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/glew_build")

include_directories(third-party/glew/include)

include_directories(third-party/stb_image)

add_subdirectory(third-party/glm)

set_property(TARGET glew_s PROPERTY FOLDER GLEW)
set_property(TARGET glew PROPERTY FOLDER GLEW)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}
    glfw
    glew_s
    glm
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR}
    ${CMAKE_BINARY_DIR}/assets
)