#include "config.hpp"

double	FRAMETIME = 0;

// Calculate the frametime of the program
static void getFrametime() {
	static double last_time = glfwGetTime();
	static int frame_count = 0;

	double current_time = glfwGetTime();
	frame_count++;
	if (current_time - last_time >= 1.0) {
		FRAMETIME = 1000.0/double(frame_count);
		frame_count = 0;
		last_time += 1.0;
	}
}

// Keep the window alive, exiting this function mean the process is over
static void program_loop(GLFWwindow *window, OBJ &obj, Shader &shaders) {
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		getFrametime();
		handleEvents(window, obj, shaders);

		obj.drawObject(shaders);

		glfwSwapBuffers(window);
	}
}

// Will call the program loop
void	RenderObject(GLFWwindow *window, OBJ &obj) {

	glfwSetWindowTitle(window, ("Scop - " + obj.getObjectData().name).c_str());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Shader shaders;
	shaders.add_shader(
		"./srcs/shaders/Standard/vertex.vert",
		"./srcs/shaders/Standard/fragment.frag",
		"Standard"
	);
	shaders.add_shader(
		"./srcs/shaders/Black&White/vertex.vert",
		"./srcs/shaders/Black&White/fragment.frag",
		"Black&White"
	);
	shaders.add_shader(
		"./srcs/shaders/Protanopia/vertex.vert",
		"./srcs/shaders/Protanopia/fragment.frag",
		"Protanopia"
	);
	shaders.add_shader(
		"./srcs/shaders/Deuteranopia/vertex.vert",
		"./srcs/shaders/Deuteranopia/fragment.frag",
		"Deuteranopia"
	);
	shaders.add_shader(
		"./srcs/shaders/Tritanopia/vertex.vert",
		"./srcs/shaders/Tritanopia/fragment.frag",
		"Tritanopia"
	);

	displayCommands();

	printVerbose("Entering program's loop");
	program_loop(window, obj, shaders);
	printVerbose("Exiting program's loop");
}