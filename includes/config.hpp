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

/// Defines
# define ROTATION_SPEED 0.05f
# define AUTOROTATION_SPEED 0.015f

/// Global variables
using namespace std;
enum class ROTATION;
class Shader;

enum class KEYBOARD_LANGUAGE {
	AZERTY,
	QWERTY
};

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern double FRAMETIME;

// Flags
extern bool VERBOSE;
extern bool RESIZABLE;
extern bool WIREFRAME;
extern ROTATION AUTOROTATE; // use ROTATION enum, NONE = manual rotation
extern bool INVERSE_AUTOROTATE;
extern KEYBOARD_LANGUAGE KEYBOARD; // use KEYBOARD_LANGUAGE enum

/// Functions
// window.cpp
GLFWwindow		*CreateWindow(OBJ &obj);
void			DestroyWindow(GLFWwindow *window);

// routine.cpp
void			RenderObject(GLFWwindow *window, OBJ &obj);

// events.cpp
void			handleEvents(GLFWwindow *window, OBJ &obj, Shader &shaders);

// flags.cpp
int				checkFlags(int argc, char **argv);

// utils.cpp
void			displayHelp(char *executable_name);
void			displayCommands();
void			printVerbose(const string &message, bool newline = true);