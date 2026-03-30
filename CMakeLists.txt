cmake_minimum_required(VERSION 3.28)
project(Platformer C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_FILES 
  ${CMAKE_SOURCE_DIR}/src/**.c
  ${CMAKE_SOURCE_DIR}/src/**.h
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# pch.h
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

# files
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Nexus_c
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/lib/Nexus_c.lib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/include)

# OpenGL
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
