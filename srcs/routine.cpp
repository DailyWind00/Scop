#include "config.hpp"

// Handle all keyboard & other events
static void	handleEvents(GLFWwindow *window) {
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Keep the window alive, exiting this function mean the process is over
static void program_loop(GLFWwindow *window, OBJ &obj, unsigned int shader) {

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glUseProgram(shader);
		handleEvents(window);
	}

	(void)obj;
}

// Will call the program loop
void	RenderObject(GLFWwindow *window, OBJ &obj) {

	unsigned int shader = make_shader(
		"./srcs/shaders/vertex.vert",
		"./srcs/shaders/fragment.frag"
	);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	printVerbose("Entering program's loop");
	program_loop(window, obj, shader);
	printVerbose("Exiting program's loop");
	
	glDeleteProgram(shader);
}