#pragma once

# include <iostream>
# include <algorithm>
# include <map>
# include "config.hpp"

using namespace std;
typedef array<float, 16> mat4;

typedef struct shaderData {
    GLuint  shaderID;
    string  vertexPath;
    string  fragmentpath;
    string  shaderName;
}   shaderData;

typedef     map<GLuint, shaderData>::iterator ShaderIterator;
typedef     pair<GLuint, shaderData> ShaderPair;

class Shader {
    private:
        map<GLuint, shaderData>     shaders;
        vector<GLuint>              shaderIDs;
        GLuint                      currentShaderID;

        GLuint  make_module(const string &filepath, GLuint module_type);
        GLuint  make_shader(const string &vertex_path, const string &fragment_path);
        void    remove_shader(GLuint shaderID);

    public:
        Shader();
        ~Shader();


        // public functions

        void    use(GLuint shaderID);
        GLuint  recompile(GLuint shaderID);
        GLuint  add_shader(const string &vertexPath, const string &fragmentPath, const string &shaderName);
        GLuint	SetNextShader();
        GLuint	SetPreviousShader();


        // Uniforms setters

        void    setBool  (GLuint &shaderID, const string &name, bool value);
        void    setInt   (GLuint &shaderID, const string &name, int value);
        void    setFloat (GLuint &shaderID, const string &name, float value);
        void    setVec2  (GLuint &shaderID, const string &name, float x, float y);
        void    setVec3  (GLuint &shaderID, const string &name, float x, float y, float z);
        void    setVec4  (GLuint &shaderID, const string &name, float x, float y, float z, float w);
        void    setMat4  (GLuint &shaderID, const string &name, mat4 matrix);


        // Getters

        const GLuint &getCurrentShaderID() const;
};