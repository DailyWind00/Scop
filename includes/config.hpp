#pragma once

/// System includes
# include <iostream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <fstream>

/// Custom includes
# include "color.h"
# include "OBJ.hpp"
# include "Shader.hpp"
# include "Matrixes.hpp"

/// Global variables
using namespace std;
enum class ROTATION;
class Shader;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// Flags
extern bool VERBOSE;
extern bool RESIZABLE;
extern bool WIREFRAME;
extern ROTATION AUTOROTATE; // use ROTATION enum, NONE = manual rotation

/// Functions
// window.cpp
GLFWwindow		*CreateWindow(OBJ &obj);
void			DestroyWindow(GLFWwindow *window);

// routine.cpp
void			RenderObject(GLFWwindow *window, OBJ &obj);

// events.cpp
void			handleEvents(GLFWwindow *window, OBJ &obj, Shader &shaders);

// utils.cpp
void			displayHelp(char *executable_name);
void			displayCommands();
void			printVerbose(const string &message, bool newline = true);