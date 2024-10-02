#include "config.hpp"

// Handle all keyboard & other events
static void	handleEvents(GLFWwindow *window) {
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Keep the window alive, exiting this function mean the process is over
static void program_loop(GLFWwindow *window, OBJ &obj, GLuint shader) {

	// tests to remove
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f,
		0.0f,  0.75f, 0.0f,
	};

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// ---

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(shader);
		glBindVertexArray(vertexbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		handleEvents(window);
		glfwSwapBuffers(window);
	}
	glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
	(void)obj;
}

// Will call the program loop
void	RenderObject(GLFWwindow *window, OBJ &obj) {

	GLuint shader = make_shader(
		"./srcs/shaders/vertex.vert",
		"./srcs/shaders/fragment.frag"
	);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	printVerbose("Entering program's loop");
	program_loop(window, obj, shader);
	printVerbose("Exiting program's loop");
	
	glDeleteProgram(shader);
}