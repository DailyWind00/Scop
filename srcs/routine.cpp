#include "config.hpp"

double FRAMETIME = 0;

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

	/// tests to remove / put in a class
	GLfloat positions[] = {
		// Front face positions
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// Back face positions
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		// Left face positions
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		// Right face positions
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		// Top face positions
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// Bottom face positions
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f
	};
	GLfloat colors[] = {
		// Front face colors
		1.0f, 0.0f, 0.0f,   // Red
		0.0f, 1.0f, 0.0f,   // Green
		0.0f, 0.0f, 1.0f,   // Blue
		1.0f, 1.0f, 0.0f,   // Yellow
		// Back face colors
		1.0f, 0.0f, 1.0f,   // Magenta
		0.0f, 1.0f, 1.0f,   // Cyan
		1.0f, 0.5f, 0.0f,   // Orange
		0.5f, 0.0f, 0.5f,   // Purple
		// Left face colors
		0.0f, 0.5f, 0.5f,   // Teal
		0.5f, 0.5f, 0.0f,   // Olive
		0.5f, 0.0f, 0.0f,   // Maroon
		0.0f, 0.0f, 0.5f,   // Navy
		// Right face colors
		0.5f, 0.5f, 0.5f,   // Gray
		0.5f, 0.5f, 1.0f,   // Light Blue
		0.5f, 1.0f, 0.5f,   // Light Green
		1.0f, 0.5f, 0.5f,   // Light Red
		// Top face colors
		0.5f, 0.0f, 0.0f,   // Dark Red
		0.0f, 0.5f, 0.0f,   // Dark Green
		0.0f, 0.0f, 0.5f,   // Dark Blue
		0.5f, 0.5f, 0.0f,   // Dark Yellow
		// Bottom face colors
		0.5f, 0.0f, 0.5f,   // Dark Magenta
		0.0f, 0.5f, 0.5f,   // Dark Cyan
		0.5f, 0.5f, 0.5f,   // Dark Gray
		0.5f, 0.5f, 0.0f    // Dark Olive
	};
	GLuint indices[] = {
		// Front face
		0, 1, 2,
		2, 3, 0,
		// Back face
		4, 5, 6,
		6, 7, 4,
		// Left face
		8, 9, 10,
		10, 11, 8,
		// Right face
		12, 13, 14,
		14, 15, 12,
		// Top face
		16, 17, 18,
		18, 19, 16,
		// Bottom face
		20, 21, 22,
		22, 23, 20
	};

	GLuint VAO; // Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    GLuint VBOs[2]; // Vertex Buffer Objects
    glGenBuffers(2, VBOs);

    // Position VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

	GLuint EBO; // Element Buffer Object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	/// ---

	shaders.use(shaders.getCurrentShaderID());

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		getFrametime();
		handleEvents(window, obj, shaders);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // TODO: change 36 to OBJ->indices.size()
		glBindVertexArray(0);

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