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
	if (VERBOSE)
		cout << "GLFW initialized\n";

	const string title = "Scop - " + obj.getObjectName();
	window = glfwCreateWindow(800, 600, title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw runtime_error("Failed to create window");
	}
	if (VERBOSE)
		cout << "Window \"" << title << "\" created\n";

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLAD");
	}

	return window;
}

// Destroy the window and terminate GLFW
// Should only be called if CreateWindow was successful
void	DestroyWindow(GLFWwindow *window) {
	if (!window)
		return;
	glfwDestroyWindow(window);
	glfwTerminate();
	if (VERBOSE)
		cout << "Window destroyed\n";
}

