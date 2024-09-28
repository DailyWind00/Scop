#include "config.hpp"

static unsigned int make_module(const string &filepath, unsigned int module_type) {
	ifstream file(filepath);
	stringstream buffer;
	string line;

	if (VERBOSE)
		cout << "Compiling shader : " << filepath;

	if (!file.is_open())
		throw runtime_error("Failed to open file " + filepath);

	while (getline(file, line))
		buffer << line << '\n';

	string shaderSourceStr = buffer.str();				// Cause corruption if 
	const char *shaderSource = shaderSourceStr.c_str(); // in a single line
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSource, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		cout << " -> " << BRed << "Error" << Color_Off << endl;
		char infoLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, infoLog);
		throw runtime_error("Failed to compile shader " + filepath + ":\n\t" + infoLog);
	}

	if (VERBOSE)
		cout << " ->" << BGreen << " Shader compiled" << Color_Off << endl;

	return shaderModule;
}

unsigned int make_shader(const string &vertex_path, const string &fragment_path) {
	vector<unsigned int> modules;
	modules.push_back(make_module(vertex_path, GL_VERTEX_SHADER));
	modules.push_back(make_module(fragment_path, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();
	for (unsigned int module : modules) {
		glAttachShader(shader, module);
		glDeleteShader(module);
	}

	if (VERBOSE)
		cout << "Linking shader";

	glLinkProgram(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		cout << " -> " << BRed << "Error" << Color_Off << endl;
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		throw runtime_error("Failed to link shader:\n\t" + *infoLog);
	}

	if (VERBOSE)
		cout << " ->" << BGreen << " Shader linked" << Color_Off << endl;

	return shader;
}