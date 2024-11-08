#include "config.hpp"

float	FOV = 45.0f;
float	RENDER_TEXTURE = 1;
float	SCALE = 1;

#define POSITIVE_PITCH_KEY_PRESSED ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define NEGATIVE_PITCH_KEY_PRESSED ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define POSITIVE_YAW_KEY_PRESSED   ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define NEGATIVE_YAW_KEY_PRESSED   ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define POSITIVE_ROLL_KEY_PRESSED  ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))
#define NEGATIVE_ROLL_KEY_PRESSED  ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::QWERTY) || (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && KEYBOARD == KEYBOARD_LANGUAGE::AZERTY))

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

static void renderTextureHandler(GLFWwindow *window, Shader &shaders) {
	static float offset = 0;

	if (offset) { // Smooth transition
		RENDER_TEXTURE = clamp(RENDER_TEXTURE + offset, 0.0f, 1.0f);
		if (RENDER_TEXTURE == 0 || RENDER_TEXTURE == 1)
			offset = 0;
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && RENDER_TEXTURE < 1) {
			printVerbose("Switching to texture rendering mode");
			offset = RENDER_TEXTURE_OFFSET_SPEED * FRAMETIME;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && RENDER_TEXTURE > 0) {
			printVerbose("Switching to vertex rendering mode");
			offset = -RENDER_TEXTURE_OFFSET_SPEED * FRAMETIME;
		}
	}
	shaders.setFloat(shaders.getCurrentShaderID(), "RenderTexture", RENDER_TEXTURE);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	SCALE = clamp(SCALE + (float)yoffset * SCROLL_SPEED, MIN_SCALE, MAX_SCALE);
	(void)window; (void)xoffset;
}

// Handle the transformation of the object, here only rotation is useful (keybind change with keyboard language)
static void transformObjectHandler(GLFWwindow *window, Shader &shaders, OBJ &obj) {
	static float x_movement, y_movement, z_movement = 0;
	static float pitch_angle, yaw_angle, roll_angle = 0;
	const vec3 &centroid = obj.getObjectData().centroid;

	/// Translation of the object
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		x_movement += TRANSLATION_SPEED * FRAMETIME * SPEED * obj.getObjectData().size;
	else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		x_movement -= TRANSLATION_SPEED * FRAMETIME * SPEED * obj.getObjectData().size;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		y_movement += TRANSLATION_SPEED * FRAMETIME * SPEED * obj.getObjectData().size;
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		y_movement -= TRANSLATION_SPEED * FRAMETIME * SPEED * obj.getObjectData().size;

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		z_movement += TRANSLATION_SPEED * FRAMETIME * SPEED * obj.getObjectData().size;
	else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		z_movement -= TRANSLATION_SPEED * FRAMETIME * SPEED * obj.getObjectData().size;

	TranslationMatrix objectTranslation( // Translate the object to the origin to rotate around the centroid
		-centroid[0] + x_movement,
		-centroid[1] + y_movement,
		-centroid[2] + z_movement
	);
	TranslationMatrix objectTranslationBack( // Translate the object back to its original position
		-centroid[0],
		-centroid[1],
		-centroid[2]
	);


	/// Rotation of the object
	if (AUTOROTATE == ROTATION::NONE) { // Manual rotation
		if (POSITIVE_PITCH_KEY_PRESSED)
			pitch_angle += ROTATION_SPEED * FRAMETIME * SPEED;
		else if (NEGATIVE_PITCH_KEY_PRESSED)
			pitch_angle -= ROTATION_SPEED * FRAMETIME * SPEED;

		if (POSITIVE_YAW_KEY_PRESSED)
			yaw_angle += ROTATION_SPEED * FRAMETIME * SPEED;
		else if (NEGATIVE_YAW_KEY_PRESSED)
			yaw_angle -= ROTATION_SPEED * FRAMETIME * SPEED;

		if (POSITIVE_ROLL_KEY_PRESSED)
			roll_angle += ROTATION_SPEED * FRAMETIME * SPEED;
		else if (NEGATIVE_ROLL_KEY_PRESSED)
			roll_angle -= ROTATION_SPEED * FRAMETIME * SPEED;
	}
	else { // Auto-rotate the object, block manual rotation

		// Update the angles based on time
		static float time = 0;

		switch (AUTOROTATE) {
			case ROTATION::PITCH:
				pitch_angle = time * SPEED;
				break;
			case ROTATION::YAW:
				yaw_angle = time * SPEED;
				break;
			case ROTATION::ROLL:
				roll_angle = time * SPEED;
				break;
			default:
				break;
		}

		time += AUTOROTATION_SPEED * FRAMETIME;
	}

	if (INVERSE_AUTOROTATE) {
		pitch_angle = -pitch_angle;
		yaw_angle = -yaw_angle;
		roll_angle = -roll_angle;
	}
	RotationMatrix objectRotation(pitch_angle, yaw_angle, roll_angle);


	// Scale of the object
	glfwSetScrollCallback(window, scroll_callback); // Modify SCALE
	ScalingMatrix objectScale(SCALE, SCALE, SCALE);

	static bool space_key_pressed = false;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (!space_key_pressed) {
			printVerbose("Reset object");
			x_movement = y_movement = z_movement = 0;
			pitch_angle = yaw_angle = roll_angle = 0;
			SCALE = 1;
			space_key_pressed = true;
		}
	}
	else
		space_key_pressed = false;

	// Combine the transformations
	Matrix transform = objectTranslationBack * objectRotation * objectScale * objectTranslation; // Order is important

	// Camera transformations
	TranslationMatrix cameraPos(centroid[0], centroid[1], centroid[2] - obj.getObjectData().size * 2.5); // Negative to be in front of the object
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
	renderTextureHandler(window, shaders);

	transformObjectHandler(window, shaders, obj);
}