#pragma once

# include "config.hpp"
# include "Matrixes.hpp"
# include <vector>
# include <array>
# include <sstream>
# include <string.h>

# define NO_INDEX (GLuint)0
using namespace std;

typedef array<GLuint, 3> Indices; // Position, Texture, Normal

typedef struct Object_Data {
	string				name;
	float				size;     // Virtual size of the edge of a cube containing the whole object, used for camera placement
	vec3				centroid; // Center of the object

	// OBJ data
	vector<GLfloat>		positions;
	vector<GLfloat>		colors;
	vector<GLfloat>		textures;
	vector<GLfloat>		normals;
	vector<Indices>		indices;  // See Indices typedef above
} Object_Data;

typedef struct Vertex { // Used to interleave vertex attributes in OBJ::setBuffers()
	vec3 position;
	vec2 texCoords;
	vec3 normal;
} Vertex;

// Class to load and draw .obj files
// The class is responsible for parsing the .obj file and setting up the buffers
// Must be created after the OpenGL context is initialized
class OBJ {
	private:
		Object_Data obj;
		GLuint VAO;
		GLuint VBO; // Position, Color, Texture, Normal
		GLuint EBO;
		GLuint TBO;

		void	parseOBJ(const string &file_name);
		void	parseMTL(const string &object_file);
		void	useTexture(const string &texture_path);
		void	setObjectSize();
		void	setObjectCentroid();
		void	setBuffers();
		void	destroyBuffers();

	public:
		OBJ(const string &file_name);
		~OBJ();


		// Public functions

		void  	drawObject();

		// Getters

		const Object_Data	&getObjectData() const;
		const GLuint		&getVAO() const;
		const GLuint		&getVBO() const;
		const GLuint		&getEBO() const;
		const GLuint		&getTBO() const;
};