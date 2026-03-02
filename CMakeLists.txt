cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_SOURCE_FILES
    src/game.cpp
    src/main.cpp
)

include_directories(src)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

add_subdirectory(engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(engine/src)

target_link_libraries(${PROJECT_NAME}
    EngineCore
)