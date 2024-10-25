cmake_minimum_required(VERSION 3.30)

project(rs-clone)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 3.4 REQUIRED)
find_package(glm 1.0.1 REQUIRED)

# GLAD files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/glad/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c
     include/glad/src/glad.c)
set(GLAD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/src/glad.c)

# stb-image.h and future files in include
file(GLOB INCLUDES include/*.h include/*.cpp)

file(GLOB SOURCES src/*.h src/*.cpp)

add_executable(${PROJECT_NAME} src/main.cpp ${GLAD_SOURCES} ${INCLUDES} ${SOURCES})

# GLFW
target_link_libraries(${PROJECT_NAME} glfw)
