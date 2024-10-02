#include "config.hpp"

static GLuint make_module(const string &filepath, GLuint module_type) {
	ifstream file(filepath);
	stringstream buffer;
	string line;

	printVerbose("Compiling shader : " + filepath + " -> ", false);

	if (!file.is_open()) {
		printVerbose((string)BRed + "Error" + Color_Off);
		throw runtime_error("Failed to open file " + filepath);
	}

	while (getline(file, line))
		buffer << line << '\n';

	string shaderSourceStr = buffer.str();				// Cause corruption if 
	const char *shaderSource = shaderSourceStr.c_str(); // in a single line
	file.close();

	GLuint shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSource, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		cout << BRed << "Error" << Color_Off << endl;
		char infoLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, infoLog);
		throw runtime_error("Failed to compile shader " + filepath + ":\n\t" + infoLog);
	}

	printVerbose((string)BGreen + "Shader compiled" + Color_Off);

	return shaderModule;
}

GLuint make_shader(const string &vertex_path, const string &fragment_path) {
	vector<GLuint> modules;
	modules.push_back(make_module(vertex_path, GL_VERTEX_SHADER));
	modules.push_back(make_module(fragment_path, GL_FRAGMENT_SHADER));

	GLuint shader = glCreateProgram();
	for (GLuint module : modules) {
		glAttachShader(shader, module);
		glDeleteShader(module);
	}

	printVerbose("Linking shader -> ", false);

	glLinkProgram(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		cout << " -> " << BRed << "Error" << Color_Off << endl;
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		throw runtime_error("Failed to link shader:\n\t" + *infoLog);
	}

	printVerbose((string)BGreen + "Shader linked" + Color_Off);

	return shader;
}