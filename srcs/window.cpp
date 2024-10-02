#include "config.hpp"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// Initialize GLFW and create a window
GLFWwindow	*CreateWindow(OBJ &obj) {
	GLFWwindow	*window;

	if (!glfwInit()) {
		cerr << BRed << "Failed to initialize GLFW" << ResetColor << '\n';
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac-os compatibility
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	printVerbose("GLFW initialized");

	const string title = "Scop - " + obj.getObjectName();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw runtime_error("Failed to create window");
	}
	printVerbose("Window \"" + title + "\" created");

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLAD");
	}
	printVerbose("GLAD initialized");

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	return window;
}

// Destroy the window and terminate GLFW
// Should only be called if CreateWindow was successful
void	DestroyWindow(GLFWwindow *window) {
	if (!window)
		return;
	glfwDestroyWindow(window);
	glfwTerminate();
	printVerbose("Window destroyed");
}

