cmake_minimum_required(VERSION 3.28)
project(my_opengl)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(SOURCES src/main.c)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)
