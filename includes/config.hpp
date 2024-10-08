#pragma once

/// system includes
# include <iostream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <fstream>

# include <glm/glm.hpp> //!\ MUST NOT APPEAR DURING CORRECTION

/// custom includes
# include "color.h"
# include "OBJ.hpp"

/// global variables
using namespace std;

/// enums
enum ROTATION {
	NONE = 0,
	PITCH,      // x
	YAW,        // y
	ROLL        // z
};

/// Flags
extern bool VERBOSE;
extern bool RESIZABLE;
extern bool WIREFRAME;
extern char AUTOROTATE; // use ROTATION enum

/// Global variables
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

/// functions
// window.cpp
GLFWwindow		*CreateWindow(OBJ &obj);
void			DestroyWindow(GLFWwindow *window);

// shader.cpp
GLuint 			make_shader(const string &vertex_path, const string &fragment_path);

// routine.cpp
void			RenderObject(GLFWwindow *window, OBJ &obj);

// utils.cpp
void			printVerbose(const string &message, bool newline = true);