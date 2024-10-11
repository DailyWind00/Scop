#include "config.hpp"

// Resize the viewport when the window is resized
static void resize_viewport(GLFWwindow *window, int width, int height) {
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
	(void)window;
}

// Handle the wireframe mode (F1 key)
static void wireframeHandler(GLFWwindow *window) {
	static bool wireframeKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		if (!wireframeKeyPressed) {
			wireframeKeyPressed = true;
			WIREFRAME = !WIREFRAME;
			glPolygonMode(GL_FRONT_AND_BACK, WIREFRAME ? GL_LINE : GL_FILL);
		}
	}
	else
		wireframeKeyPressed = false;
}

// Handle the recompilation of the current shader (F5 key)
static void recompilationHandler(GLFWwindow *window, Shader &shaders) {
	static bool recompileKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
		if (!recompileKeyPressed) {
			recompileKeyPressed = true;
			shaders.recompile(shaders.getCurrentShaderID());
		}
	}
	else
		recompileKeyPressed = false;
}

// Handle the shader switch (left & right arrow keys)
static void shaderSwitchHandler(GLFWwindow *window, Shader &shaders) {
	static bool changeShaderKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (!changeShaderKeyPressed) {
			changeShaderKeyPressed = true;
			shaders.SetNextShader();
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (!changeShaderKeyPressed) {
			changeShaderKeyPressed = true;
			shaders.SetPreviousShader();
		}
	}
	else
		changeShaderKeyPressed = false;
}

// Handle all keyboard & other events
void	handleEvents(GLFWwindow *window, OBJ &obj, Shader &shaders) {
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (RESIZABLE)
		glfwSetFramebufferSizeCallback(window, resize_viewport);

	wireframeHandler(window);
	recompilationHandler(window, shaders);
	shaderSwitchHandler(window, shaders);

	(void)obj; // to remove after obj parsing
}