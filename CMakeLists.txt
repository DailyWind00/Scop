cmake_minimum_required(VERSION 3.0.0)
project(Scop VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(Scop 
	srcs/main.cpp 
	includes/classes/OBJ.cpp
	dependencies/glad/glad.c
)

target_include_directories(Scop PRIVATE dependencies)
target_include_directories(Scop PRIVATE includes)
target_include_directories(Scop PRIVATE includes/classes)

target_link_libraries(Scop glfw OpenGL::GL)