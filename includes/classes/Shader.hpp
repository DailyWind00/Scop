#pragma once

# include <iostream>
# include <map>
# include "config.hpp"

using namespace std;

typedef struct shaderData {
    GLuint  shaderID;
    string  vertexPath;
    string  fragmentpath;
    string  shaderName;   // optional
}   shaderData;

typedef  map<GLuint, shaderData>::iterator ShaderPair;

class Shader {
    private:
        map<GLuint, shaderData>  shaders;

        GLuint  make_module(const string &filepath, GLuint module_type);
        GLuint  make_shader(const string &vertex_path, const string &fragment_path);
        void    remove_shader(GLuint shaderID);

    public:
        Shader();
        ~Shader();

        // public functions

        void    use(GLuint shaderID);
        GLuint  recompile(GLuint shaderID);
        GLuint  add_shader(const string &vertexPath, const string &fragmentPath, const string &shaderName = NULL);

};