#pragma once

/// System includes
# include <iostream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <stb/stb_image.h>
# include <fstream>

/// Custom includes
# define COLOR_HEADER_CXX
# include "color.hpp"
# include "Matrixes.hpp"
# include "Shader.hpp"
class Shader;
# include "OBJ.hpp"
class OBJ;

/// Defines
# define ROTATION_SPEED 0.05f
# define MOUSE_ROTATION_SPEED 0.004f
# define TRANSLATION_SPEED 0.001f
# define AUTOROTATION_SPEED 0.015f
# define RENDER_TEXTURE_OFFSET_SPEED 0.005f
# define SCROLL_SPEED 0.1f
# define MIN_SCALE 0.1f
# define MAX_SCALE 10.0f

/// Global variables
using namespace std;
enum class ROTATION;

enum class KEYBOARD_LANGUAGE {
	AZERTY,
	QWERTY
};

typedef struct mouse_data {
	double	cur_x;
	double	cur_y;
	double	last_x;
	double	last_y;
	bool	left_button_pressed;
	bool	right_button_pressed;
} mouse_data;

extern int			WINDOW_WIDTH;
extern int			WINDOW_HEIGHT;
extern double		FRAMETIME;
extern float		FOV;
extern float		RENDER_TEXTURE; // between 0 and 1 : 0 = no texture, 1 = texture (smooth transition required)
extern float		SCALE;
extern mouse_data	MOUSE;

// Flags (default values are set in flags.cpp)
extern bool					VERBOSE;
extern bool					RESIZABLE;
extern bool					WIREFRAME;
extern float				SPEED;
extern ROTATION				AUTOROTATE;          // use ROTATION enum, NONE = manual rotation
extern bool					INVERSE_AUTOROTATE;
extern KEYBOARD_LANGUAGE	KEYBOARD;            // use KEYBOARD_LANGUAGE enum
extern bool					SHOW_TOOLTIP;

/// Functions
// window.cpp
GLFWwindow		*CreateWindow();
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
ifstream		openReadFile(const string &filename);
unsigned char	*stbi_loader(const string &filename, int &width, int &height, int &nrChannels);
void			printVerbose(const string &message, bool newline = true);
float			clamp(float value, float min, float max);
float			toRadians(float degrees);