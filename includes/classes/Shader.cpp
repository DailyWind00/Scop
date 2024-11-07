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

	printVerbose("> Compiling shader : " + filepath + " -> ", false);

	if (!file.is_open()) {
		printVerbose(BRed + "Error" + Color_Off);
		throw runtime_error("Failed to open file " + filepath);
	}

	while (getline(file, line))
		buffer << line << '\n';

	string shaderSourceStr = buffer.str();				// Cause corruption if 
	const char *shaderSource = shaderSourceStr.c_str(); // in a single line
	file.close();

	GLuint shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSource, nullptr);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		printVerbose(BRed + "Error" + Color_Off);
		string infoLog;
		infoLog.resize(1024);
		glGetShaderInfoLog(shaderModule, 1024, nullptr, (GLchar *)infoLog.data());
		throw runtime_error("Failed to compile shader " + filepath + ":\n\t" + infoLog);
	}

	printVerbose(BGreen + "Shader compiled" + Color_Off);

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

	printVerbose("> Linking shader -> ", false);

	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		printVerbose(BRed + "Error" + Color_Off);
		string infoLog;
		infoLog.resize(1024);
		glGetProgramInfoLog(shader, 1024, nullptr, (GLchar *)infoLog.data());
		throw runtime_error("Failed to link shader:\n\t" + infoLog);
	}

	printVerbose(BGreen + "Shader linked" + Color_Off);

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
// Return the id of the new recompiled shader
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
// By default, the first shader added is the current shader
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
	if (shaders.size() == 1 || currentShaderID == 0)
		glUseProgram(data.shaderID);

	return data.shaderID;
}

// Use the next shader in the vector as the current shader
// Return the id of the new current shader in the vector
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
// Return the id of the new current shader in the vector
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



/// Uniforms setters

// Bool
void    Shader::setBool(const string &name, bool value) {
	glUniform1i(glGetUniformLocation(currentShaderID, name.c_str()), (int)value);
}
void    Shader::setBool(const GLuint &shaderID, const string &name, bool value) {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}

// Int
void    Shader::setInt(const string &name, int value) {
	glUniform1i(glGetUniformLocation(currentShaderID, name.c_str()), value);
}
void    Shader::setInt(const GLuint &shaderID, const string &name, int value) {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

// Float
void    Shader::setFloat(const string &name, float value) {
	glUniform1f(glGetUniformLocation(currentShaderID, name.c_str()), value);
}
void    Shader::setFloat(const GLuint &shaderID, const string &name, float value) {
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

// Vec2
void    Shader::setVec2(const string &name, float x, float y) {
	glUniform2f(glGetUniformLocation(currentShaderID, name.c_str()), x, y);
}
void    Shader::setVec2(const GLuint &shaderID, const string &name, float x, float y) {
	glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
}
void    Shader::setVec2(const string &name, vec2 vector) {
	glUniform2f(glGetUniformLocation(currentShaderID, name.c_str()), vector[0], vector[1]);
}
void    Shader::setVec2(const GLuint &shaderID, const string &name, vec2 vector) {
	glUniform2f(glGetUniformLocation(shaderID, name.c_str()), vector[0], vector[1]);
}

// Vec3
void    Shader::setVec3(const string &name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(currentShaderID, name.c_str()), x, y, z);
}
void    Shader::setVec3(const GLuint &shaderID, const string &name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
}
void    Shader::setVec3(const string &name, vec3 vector) {
	glUniform3f(glGetUniformLocation(currentShaderID, name.c_str()), vector[0], vector[1], vector[2]);
}
void    Shader::setVec3(const GLuint &shaderID, const string &name, vec3 vector) {
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vector[0], vector[1], vector[2]);
}

// Vec4
void    Shader::setVec4(const string &name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(currentShaderID, name.c_str()), x, y, z, w);
}
void    Shader::setVec4(const GLuint &shaderID, const string &name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w);
}
void    Shader::setVec4(const string &name, vec4 vector) {
	glUniform4f(glGetUniformLocation(currentShaderID, name.c_str()), vector[0], vector[1], vector[2], vector[3]);
}
void    Shader::setVec4(const GLuint &shaderID, const string &name, vec4 vector) {
	glUniform4f(glGetUniformLocation(shaderID, name.c_str()), vector[0], vector[1], vector[2], vector[3]);
}

// Mat4
void	Shader::setMat4(const string &name, mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(currentShaderID, name.c_str()), 1, GL_FALSE, &matrix[0]);
}
void	Shader::setMat4(const GLuint &shaderID, const string &name, mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &matrix[0]);
}
/// ---



/// Getters

// Return the id of the current shader
// By default, the first shader added is the current shader
const GLuint &Shader::getCurrentShaderID() const {
	return currentShaderID;
}
/// ---