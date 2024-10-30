#pragma once

# include "config.hpp"
# include "Matrixes.hpp"
# include <vector>
# include <array>
# include <map>
# include <sstream>
# include <string.h>

# define DEFAULT_COLORS (float[3]){0.8f, 0.6f, 0.4f}
# define NO_INDEX (GLuint)0
# define NO_TEXTURE (unsigned short)-1
using namespace std;

typedef array<GLuint, 3> Indice; // Position, Texture, Normal

// Used to interleave vertex attributes in OBJ::setBuffers()
typedef struct Vertex {
	vec3 position;
	vec3 color;
	vec2 texCoords;
	vec3 normal;
} Vertex;

// Data structures for .mtl files
typedef struct Material {
	string			name;
	string			texture_path;
	unsigned short	texture_index;
	// ...
} mtl_Data;

// Shapes are the different parts of an object
// A shape is created when a "g" line is found in the .obj file
typedef struct Shape {
	string			name;   
	vector<Indice>	indices;
	string			material_name;
	unsigned short	material_index = NO_TEXTURE; // Index of Object::materials
} Shape;

// Raw vertex attributes
typedef struct Attributes {
	vector<GLfloat>	positions;
	vector<GLfloat>	colors;    // See DEFAULT_COLORS define
	vector<GLfloat>	textures;
	vector<GLfloat> normals;
} Attributes;

// Data structures for .obj files
typedef struct Object {
	string	name;     // Name of the object (first occurernce of "o" in the file)
	float	size;     // Virtual size of the edge of a cube containing the whole object, used for camera placement
	vec3	centroid; // Center of the object

	Attributes			attributes; // Interleaved vertex attributes
	vector<Shape>		shapes;
	vector<Material>	materials; // Use Shape::material_index for correct Shape/Texture loading
} Object;

// Class to load and draw .obj files
// The class is responsible for parsing the .obj file and setting up the buffers
// Must be created after the OpenGL context is initialized
class OBJ {
	private:
		Object 			obj;
		GLuint 			VAO;
		GLuint 			VBO; // Position, Color, Texture, Normal
		GLuint 			EBO;
		vector<GLuint>	TBO;


		// Parsers

		void	parseOBJ(const string &file_name);
		void	parseMTL(const string &object_file);


		// Privates functions

		void	setObjectTextures();
		void	setObjectSize();
		void	setObjectCentroid();
		void	setBuffers();
		void	destroyBuffers();

	public:
		OBJ(const string &file_name);
		~OBJ();


		// Public functions

		void  	drawObject(Shader &shader);


		// Getters

		const Object	&getObjectData() const;
};