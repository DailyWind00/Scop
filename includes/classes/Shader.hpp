#pragma once

# include <iostream>
# include <algorithm>
# include <vector>
# include <map>
# include "config.hpp"
# include "Matrixes.hpp"

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


        /// public functions

        void    use(GLuint shaderID);
        GLuint  recompile(GLuint shaderID);
        GLuint  add_shader(const string &vertexPath, const string &fragmentPath, const string &shaderName);
        GLuint	SetNextShader();
        GLuint	SetPreviousShader();


        /// Uniforms setters

        // Bool
        void    setBool  (const string &name, bool value);
        void    setBool  (const GLuint &shaderID, const string &name, bool value);

        // Int
        void    setInt   (const string &name, int value);
        void    setInt   (const GLuint &shaderID, const string &name, int value);

        // Float
        void    setFloat (const string &name, float value);
        void    setFloat (const GLuint &shaderID, const string &name, float value);

        // Vec2
        void    setVec2  (const string &name, float x, float y);
        void    setVec2  (const GLuint &shaderID, const string &name, float x, float y);
        void    setVec2  (const string &name, vec2 vector);
        void    setVec2  (const GLuint &shaderID, const string &name, vec2 vector);

        // Vec3
        void    setVec3  (const string &name, float x, float y, float z);
        void    setVec3  (const GLuint &shaderID, const string &name, float x, float y, float z);
        void    setVec3  (const string &name, vec3 vector);
        void    setVec3  (const GLuint &shaderID, const string &name, vec3 vector);

        // Vec4
        void    setVec4  (const string &name, float x, float y, float z, float w);
        void    setVec4  (const GLuint &shaderID, const string &name, float x, float y, float z, float w);
        void    setVec4  (const string &name, vec4 vector);
        void    setVec4  (const GLuint &shaderID, const string &name, vec4 vector);

        // Mat4
        void    setMat4  (const string &name, mat4 matrix);
        void    setMat4  (const GLuint &shaderID, const string &name, mat4 matrix);


        /// Getters

        const GLuint &getCurrentShaderID() const;
};