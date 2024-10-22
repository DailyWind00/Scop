#pragma once

/// System includes
# include <iostream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <stb/stb_image.h>
# include <fstream>

/// Custom includes
# include "color.h"
# include "OBJ.hpp"
# include "Shader.hpp"
# include "Matrixes.hpp"

/// Defines
# define ROTATION_SPEED 0.05f
# define AUTOROTATION_SPEED 0.015f
# define ZOOM_SPEED 0.1f
# define MAX_ZOOM 10.0
# define MIN_ZOOM 1.0

/// Global variables
using namespace std;
enum class ROTATION;
class Shader;
class OBJ;

enum class KEYBOARD_LANGUAGE {
	AZERTY,
	QWERTY
};

extern int		WINDOW_WIDTH;
extern int		WINDOW_HEIGHT;
extern double	FRAMETIME;
extern float	FOV;
extern float	RENDER_TEXTURE; // 0 = no texture, 1 = texture (smooth transition required)
extern float	ZOOM;

// Flags (default values are set in flags.cpp)
extern bool					VERBOSE;
extern bool					RESIZABLE;
extern bool					WIREFRAME;
extern ROTATION				AUTOROTATE;          // use ROTATION enum, NONE = manual rotation
extern bool					INVERSE_AUTOROTATE;
extern KEYBOARD_LANGUAGE	KEYBOARD;            // use KEYBOARD_LANGUAGE enum
extern bool					SHOW_TOOLTIP;

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