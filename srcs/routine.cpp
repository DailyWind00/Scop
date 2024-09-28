#include "config.hpp"

static void program_loop(GLFWwindow *window, OBJ &obj, unsigned int shader) {
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glUseProgram(shader);
		glfwPollEvents();
	}
	(void)obj;
}

// Will call the program loop
void	RenderObject(GLFWwindow *window, OBJ &obj) {

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	unsigned int shader = make_shader( // Will probably move to CreateWindow
		"./srcs/shaders/vertex.vert",
		"./srcs/shaders/fragment.frag"
	);

	program_loop(window, obj, shader);
	glDeleteProgram(shader); // Will probably move to DestroyWindow
}