#pragma once

// system includes
# include <iostream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <fstream>

// custom includes
# include "color.h"
# include "OBJ.hpp"

// global variables
using namespace std;

extern bool VERBOSE;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// functions
GLFWwindow	*CreateWindow(OBJ &obj);
void		DestroyWindow(GLFWwindow *window);