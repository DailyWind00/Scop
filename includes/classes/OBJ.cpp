#include "OBJ.hpp"

/// Constructors & Destructors 
OBJ::OBJ(const string &file_name) {
	if (file_name.size() < 4 || file_name.substr(file_name.size() - 4) != ".obj")
		throw runtime_error("Error while opening object file : invalid extension");

	try {
		printVerbose("Parsing object file : " + file_name);
		parseOBJ(file_name);
		printVerbose("Object file parsed");
	}
	catch(const std::exception& e) {
		throw runtime_error("Error in " + file_name + " : " + e.what());
	}
	setBuffers();
}

OBJ::~OBJ() {
	if (!obj.shapes.empty())
		destroyBuffers();
}
/// ---



/// Privates functions
void	OBJ::useTexture(const string &texture_path) {
	int width, height, nrChannels;
	unsigned char *data = stbi_loader(texture_path, width, height, nrChannels);

	GLuint TBO;
	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void	OBJ::setObjectSize() {
    if (obj.attributes.positions.empty()) {
        obj.size = 0.0f;
        return;	
    }

	vec3 max_ = {0.0f, 0.0f, 0.0f};
	vec3 min_ = {0.0f, 0.0f, 0.0f};
    size_t numVertices = obj.attributes.positions.size() / 3;

    for (size_t i = 0; i < numVertices; ++i) {
        max_[0] = max(max_[0], obj.attributes.positions[3 * i]);
		max_[1] = max(max_[1], obj.attributes.positions[3 * i + 1]);
		max_[2] = max(max_[2], obj.attributes.positions[3 * i + 2]);

		min_[0] = min(min_[0], obj.attributes.positions[3 * i]);
		min_[1] = min(min_[1], obj.attributes.positions[3 * i + 1]);
		min_[2] = min(min_[2], obj.attributes.positions[3 * i + 2]);
    }

	obj.size = max(max_[0] - min_[0], max(max_[1] - min_[1], max_[2] - min_[2]));
}

void	OBJ::setObjectCentroid() {
    if (obj.attributes.positions.empty()) {
        obj.centroid = {0.0f, 0.0f, 0.0f};
        return;	
    }

    vec3 sum = {0.0f, 0.0f, 0.0f};
    size_t numVertices = obj.attributes.positions.size() / 3;

    for (size_t i = 0; i < numVertices; ++i) {
        sum[0] += obj.attributes.positions[3 * i];
        sum[1] += obj.attributes.positions[3 * i + 1];
        sum[2] += obj.attributes.positions[3 * i + 2];
    }

    obj.centroid[0] = sum[0] / numVertices;
    obj.centroid[1] = sum[1] / numVertices;
    obj.centroid[2] = sum[2] / numVertices;
}

void	OBJ::setBuffers() {
	printVerbose("Setting GL buffers");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Interleave vertex attributes
    std::vector<Vertex> vertexes;
	int color = 0;

	// for (const Material &mat : obj.materials) {
	// 	cout << "Material : " << mat.name << endl;
	// 	cout << "Path     : " << mat.texture_path << endl;
	// 	cout << "Index    : " << mat.texture_index << endl;
	// 	cout << "Texture  : " << TBO[mat.texture_index] << endl;
	// }
	printVerbose("Loading " + to_string(obj.shapes.size()) + " shapes :");
	for (const Shape &shape : obj.shapes) {
		printVerbose("| Shape : " + shape.name + " - Indices : " + to_string(shape.indices.size()));
		for (const Indice &indice : shape.indices) {
			Vertex vertex;
			vertex.position  = {obj.attributes.positions[3 * indice[0]], obj.attributes.positions[3 * indice[0] + 1], obj.attributes.positions[3 * indice[0] + 2]};
			vertex.color     = {DEFAULT_COLORS[color % 3], DEFAULT_COLORS[color % 3], DEFAULT_COLORS[color % 3]};
			if (indice[1] != NO_INDEX)
				vertex.texCoords = {obj.attributes.textures[2 * indice[1]], obj.attributes.textures[2 * indice[1] + 1]};
			if (indice[2] != NO_INDEX)
				vertex.normal    = {obj.attributes.normals[3 * indice[2]], obj.attributes.normals[3 * indice[2] + 1], obj.attributes.normals[3 * indice[2] + 2]};
			vertexes.push_back(vertex);
			color++;
		}
	}

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex), &vertexes[0], GL_STATIC_DRAW);

    // Bind and set EBO
    std::vector<GLuint> elementIndices;
	for (size_t i = 0; i < vertexes.size(); i++)
		elementIndices.push_back(i);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementIndices.size() * sizeof(GLuint), &elementIndices[0], GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

	printVerbose("GL buffers set");
}

void	OBJ::destroyBuffers() {
	printVerbose("Destroying GL buffers");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &TBO);

	printVerbose("GL buffers destroyed");
}
/// ---



/// Public functions

// Draw the object
void	OBJ::drawObject() {
	glBindVertexArray(VAO);
	for (const Shape &shape : obj.shapes) {
		// if (!shape.material.texture_path.empty())
			// Use the texture with shape.material.texture_id
		glDrawElements(GL_TRIANGLES, shape.indices.size() * 6, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}
/// ---



/// Getters
const Object &OBJ::getObjectData() const {
	return obj;
}
/// ---