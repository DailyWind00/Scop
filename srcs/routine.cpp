#include "config.hpp"

static void resize_viewport(GLFWwindow *window, int width, int height) {
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
	(void)window;
}

// Handle all keyboard & other events
static void	handleEvents(GLFWwindow *window, OBJ &obj, Shader &shaders) {
	glfwPollEvents();

	if (RESIZABLE)
		glfwSetFramebufferSizeCallback(window, resize_viewport);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static bool wireframeKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		if (!wireframeKeyPressed) {
			if (WIREFRAME) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				printVerbose("Wireframe mode disabled");
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				printVerbose("Wireframe mode enabled");
			}
			WIREFRAME = !WIREFRAME;
			wireframeKeyPressed = true;
		}
	}
	else
		wireframeKeyPressed = false;

	// if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
	// 	GLuint newID = shaders.recompile(0);
	// 	shaders.use(newID);
	// }

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

	(void)shaders;
	(void)obj; // to remove after obj parsing
}

// Keep the window alive, exiting this function mean the process is over
static void program_loop(GLFWwindow *window, OBJ &obj, Shader &shaders) {

	/// tests to remove / put in a class
    GLfloat positions[] = {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
       -0.5f, -0.5f, 0.0f,  // Bottom Left
       -0.5f,  0.5f, 0.0f   // Top Left 
    };
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,   // Red
        0.0f, 1.0f, 0.0f,   // Green
        0.0f, 0.0f, 1.0f,   // Blue
        1.0f, 1.0f, 0.0f    // Yellow
    };
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VAO; // Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    GLuint VBOs[2]; // Vertex Buffer Objects
    glGenBuffers(2, VBOs);

    // Position VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); // May change to dynamic draw
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); // May change to dynamic draw
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

	GLuint EBO; // Element Buffer Object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // May change to dynamic draw

	glBindVertexArray(0);
	/// ---

	shaders.use(shaders.getShaderIDs()[0]);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		handleEvents(window, obj, shaders);
		glfwSwapBuffers(window);
	}

    // Cleanup
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
	(void)obj; // to remove after obj parsing
}

// Will call the program loop
void	RenderObject(GLFWwindow *window, OBJ &obj) {

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

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