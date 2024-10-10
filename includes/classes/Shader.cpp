#include "Shader.hpp"

/// Constructors & Destructors 
Shader::Shader() {
}

Shader::~Shader() {
	glUseProgram(0);

	ShaderIterator	it;
	while (!shaders.empty()) {
		it = shaders.begin();
		remove_shader(it->first);
	}
	shaders.clear();
}
/// ---



/// Privates functions

// Should never be called outside of make_shader()
GLuint Shader::make_module(const string &filepath, GLuint module_type) {
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

GLuint Shader::make_shader(const string &vertex_path, const string &fragment_path) {
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

void	Shader::remove_shader(GLuint shaderID) {
	glDeleteProgram(shaderID);
	shaderIDs.erase(remove(shaderIDs.begin(), shaderIDs.end(), shaderID), shaderIDs.end());
	shaders.erase(shaderID);
}
/// ---



/// Public functions

// Use the shader given with the shaderID,
void Shader::use(GLuint shaderID) {
    if (shaders.empty()) {
        cerr << BRed << "Error: No shaders available to use." << ResetColor << endl;
        return;
    }

    auto it = shaders.find(shaderID);
    if (it == shaders.end()) {
        cerr << BRed << "Error: Shader ID " << shaderID << " not found." << ResetColor << endl;
        return;
    }

    glUseProgram(it->first);
	currentShaderID = it->first;

    printVerbose("Now using shader " + to_string(it->first) + " \"" + it->second.shaderName + "\"");
}

// Recompile the shader given with the shaderID
GLuint	Shader::recompile(GLuint shaderID) {
	ShaderIterator	shader = shaders.find(shaderID);
	if (shader == shaders.end()) {
		cerr << BRed << "Shader recompilation error : Shader ID " << shaderID << " not found." << ResetColor << endl;
		return 0;
	}

	printVerbose("Recompiling shader \"" + shader->second.shaderName + "\" ...");

	glUseProgram(0);

	GLuint newID = 0;
	try {
		newID = make_shader(shader->second.vertexPath, shader->second.fragmentpath);
		shaders.insert(ShaderPair(newID, shader->second));
		remove_shader(shaderID);
		currentShaderID = newID;
		glUseProgram(currentShaderID);
	}
	catch(const std::exception& e) {
		cerr << BRed << "Shader recompilation error : " << e.what() << ResetColor <<  endl;
		glUseProgram(currentShaderID);
		return 0;
	}

	printVerbose("Recompilation done");
	return newID;
}

// Add a new shader to the Shaders class, return the id of the new shader
GLuint	Shader::add_shader(const string &vertexPath, const string &fragmentPath, const string &shaderName) {
	printVerbose("Creating shader \"" + shaderName + "\"");
	shaderData	data = {
		make_shader(vertexPath, fragmentPath),
		vertexPath,
		fragmentPath,
		shaderName
	};
	shaders.insert(ShaderPair(data.shaderID, data));
	shaderIDs.push_back(data.shaderID);

	printVerbose("Added shader \"" + shaderName + "\" with ID " + to_string(data.shaderID));

	if (shaders.size() == 1)
		currentShaderID = data.shaderID;

	return data.shaderID;
}

// Use the next shader in the vector as the current shader
// Return the id of the next shader in the vector
GLuint	Shader::SetNextShader() {
	if (shaders.empty()) {
		cerr << BRed << "Error: No shaders available to use." << ResetColor << endl;
		return 0;
	}

	auto	it = shaders.find(currentShaderID);
	if (it == shaders.end()) {
		cerr << BRed << "Error: Shader ID " << currentShaderID << " not found." << ResetColor << endl;
		return 0;
	}
	if (++it == shaders.end())
		it = shaders.begin();

	printVerbose("Now using shader " + to_string(it->first) + " \"" + it->second.shaderName + "\"");
	currentShaderID = it->first;
	glUseProgram(currentShaderID);

	return currentShaderID;
}

// Use the previous shader in the vector as the current shader
// Return the id of the next shader in the vector
GLuint	Shader::SetPreviousShader() {
	if (shaders.empty()) {
		cerr << BRed << "Error: No shaders available to use." << ResetColor << endl;
		return 0;
	}

	auto	it = shaders.find(currentShaderID);
	if (it == shaders.end()) {
		cerr << BRed << "Error: Shader ID " << currentShaderID << " not found." << ResetColor << endl;
		return 0;
	}
	if (it == shaders.begin())
		it = shaders.end();
	--it;

	printVerbose("Now using shader " + to_string(it->first) + " \"" + it->second.shaderName + "\"");
	currentShaderID = it->first;
	glUseProgram(currentShaderID);

	return currentShaderID;
}
/// ---



/// Getters

// Return the id of the current shader
// By default, the first shader added is the current shader
const GLuint &Shader::getCurrentShaderID() const {
	return currentShaderID;
}
/// ---