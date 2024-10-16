#include "config.hpp"

float FOV = 45.0f;

#define POSITIVE_PITCH_KEY_PRESSED ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define NEGATIVE_PITCH_KEY_PRESSED ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define POSITIVE_YAW_KEY_PRESSED   ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define NEGATIVE_YAW_KEY_PRESSED   ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define POSITIVE_ROLL_KEY_PRESSED  ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define NEGATIVE_ROLL_KEY_PRESSED  ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))

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

// Handle the transformation of the object, here only rotation is useful (keybind change with keyboard language)
static void transformObjectHandler(GLFWwindow *window, Shader &shaders) {
	static float pitch_angle = 0;
	static float yaw_angle = 0;
	static float roll_angle = 0;

	if (AUTOROTATE == ROTATION::NONE) { // Manual rotation
		if (POSITIVE_PITCH_KEY_PRESSED)
			pitch_angle += ROTATION_SPEED * FRAMETIME;
		else if (NEGATIVE_PITCH_KEY_PRESSED)
			pitch_angle -= ROTATION_SPEED * FRAMETIME;

		if (POSITIVE_YAW_KEY_PRESSED)
			yaw_angle += ROTATION_SPEED * FRAMETIME;
		else if (NEGATIVE_YAW_KEY_PRESSED)
			yaw_angle -= ROTATION_SPEED * FRAMETIME;

		if (POSITIVE_ROLL_KEY_PRESSED)
			roll_angle += ROTATION_SPEED * FRAMETIME;
		else if (NEGATIVE_ROLL_KEY_PRESSED)
			roll_angle -= ROTATION_SPEED * FRAMETIME;
	}
	else { // Auto-rotate the object, block manual rotation

		// Update the angles based on time
		static float time = 0;

		switch (AUTOROTATE) {
			case ROTATION::PITCH:
				pitch_angle = time;
				break;
			case ROTATION::YAW:
				yaw_angle = time;
				break;
			case ROTATION::ROLL:
				roll_angle = time;
				break;
			default:
				break;
		}

		time += AUTOROTATION_SPEED * FRAMETIME;
	}

	// Rotations of the object
	if (INVERSE_AUTOROTATE) {
		pitch_angle = -pitch_angle;
		yaw_angle = -yaw_angle;
		roll_angle = -roll_angle;
	}
	RotationMatrix pitch(ROTATION::PITCH, pitch_angle);
	RotationMatrix yaw(ROTATION::YAW, yaw_angle);
	RotationMatrix roll(ROTATION::ROLL, roll_angle);

	Matrix transform = pitch * yaw * roll; // TODO: put in the obj class

	// Camera transformations
	TranslationMatrix cameraPos(0.0f, 0.0f, -5.0f); // TODO: Change z value to object size radius * 2 + 10%
	RotationMatrix    cameraAngle(0.0f, 0.0f, 0.0f);

	Matrix view = cameraAngle * cameraPos;

	// Projection matrix
	float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	ProjectionMatrix projection(FOV, aspectRatio, 0.1f, 100.0f);

	// Send the combined transformation matrixes to the shader
	shaders.setMat4(shaders.getCurrentShaderID(), "Transform", transform.getMatrix());
	shaders.setMat4(shaders.getCurrentShaderID(), "View", view.getMatrix());
	shaders.setMat4(shaders.getCurrentShaderID(), "Projection", projection.getMatrix());
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
	transformObjectHandler(window, shaders);

	(void)obj; // to remove after obj parsing
}