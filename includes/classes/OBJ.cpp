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
	destroyBuffers();
}
/// ---



/// Privates functions
void	OBJ::setObjectTextures() {
	if (obj.materials.empty()) {
		cout << BYellow << "Notice : No texture is set, using vertex render mode as default" << ResetColor << endl;
		return;
	}

	TBO.resize(obj.materials.size());
	glGenTextures(obj.materials.size(), TBO.data());

	int current = 0;
	for (Material &mat : obj.materials) {
		int count = 0;
		for (const Shape &shapes : obj.shapes)
			count += (shapes.material_name == mat.name); // Count the number of shapes using this material
		if (!count) {
			cout << BYellow << "Notice : No shape is using material \"" << mat.name << "\"" << ResetColor << endl;
			continue ;
		}

		if (current >= MAX_TEXTURES_COUNT) { // Maximum number of textures reached
			static bool warning = false;
			if (!warning) {
				cout << BOrange << "Warning : Maximum number of textures reached" << ResetColor << endl;
				cout << BCyan   << "Tip : Consider using texture atlas"           << ResetColor << endl;
				warning = true;
			}
			continue;
		}

		int width, height, nrChannels;
		unsigned char *data = nullptr;

		if (mat.texture_path.empty()) {
			mat.texture_index = NO_TEXTURE; // No texture for this material
			continue ;
		}

		data = stbi_loader(mat.texture_path, width, height, nrChannels);
		if (!data) {
			cout << BOrange << "Warning : Failed to load texture \"" << mat.texture_path << "\"" << ResetColor << endl;
			mat.texture_index = NO_TEXTURE; // Failed to load texture
			stbi_image_free(data);
			continue ;
		}
		glBindTexture(GL_TEXTURE_2D, TBO[current]);

		// Set texture parameters after binding the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else {
			stbi_image_free(data);
			throw runtime_error("Error while loading texture : invalid number of channels");
		}
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		// Set the texture index for each shape
		for (Shape &shapes : obj.shapes) {
			if (shapes.material_name == mat.name)
				shapes.material_index = current;
		}
		mat.texture_index = current++;
	}
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

    // Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Element Buffer Object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Interleave vertex attributes (positions, colors, texCoords, normals)
    vector<GLfloat>	interleavedData;
	vector<GLuint>	elementIndices;
	int i = 0;

	printVerbose("Loading " + to_string(obj.shapes.size()) + " shapes :");
	for (const Shape &shape : obj.shapes) {
		printVerbose("| Shape : " + shape.name + " - Indices : " + to_string(shape.indices.size()));
		for (const Indice &indice : shape.indices) {
			// Positions
			interleavedData.push_back(obj.attributes.positions[3 * indice[0]]);
			interleavedData.push_back(obj.attributes.positions[3 * indice[0] + 1]);
			interleavedData.push_back(obj.attributes.positions[3 * indice[0] + 2]);

			// Colors
			interleavedData.push_back(DEFAULT_COLORS[i % 3]);
			interleavedData.push_back(DEFAULT_COLORS[i % 3]);
			interleavedData.push_back(DEFAULT_COLORS[i % 3]);

			// Texture coordinates
			if (indice[1] != NO_INDEX) {
				interleavedData.push_back(obj.attributes.textures[2 * indice[1]]);
				interleavedData.push_back(obj.attributes.textures[2 * indice[1] + 1]);
			}
			else {
				interleavedData.push_back(NO_INDEX);
				interleavedData.push_back(NO_INDEX);
			}

			// Normals
			if (indice[2] != NO_INDEX) {
				interleavedData.push_back(obj.attributes.normals[3 * indice[2]]);
				interleavedData.push_back(obj.attributes.normals[3 * indice[2] + 1]);
				interleavedData.push_back(obj.attributes.normals[3 * indice[2] + 2]);
			}
			else {
				interleavedData.push_back(NO_INDEX);
				interleavedData.push_back(NO_INDEX);
				interleavedData.push_back(NO_INDEX);
			}

			elementIndices.push_back(i++);
    	}
	}

	glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(GLfloat), interleavedData.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementIndices.size() * sizeof(GLuint), elementIndices.data(), GL_STATIC_DRAW);

	// Define the layout of the vertex attributes
    GLsizei stride = (3 + 3 + 2 + 3) * sizeof(GLfloat); // position + color + texCoords + normal

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Normal attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

	setObjectTextures();

	printVerbose("GL buffers set");
}

void	OBJ::destroyBuffers() {
	printVerbose("Destroying GL buffers");

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(TBO.size(), TBO.data());

	printVerbose("GL buffers destroyed");
}
/// ---



/// Public functions

// Draw the object
void	OBJ::drawObject(Shader &shader) {
	if (obj.materials.empty()) // Auto Vertex render mode if no texture
		shader.setFloat("RenderTexture", 0);

	for (const Shape &shape : obj.shapes) {
		if (shape.material_index != NO_TEXTURE) {
			shader.setInt("textureDiffuse", (GLint)shape.material_index);
			glActiveTexture(GL_TEXTURE0 + shape.material_index);
			glBindTexture(GL_TEXTURE_2D, TBO[shape.material_index]);
		}
		else // Auto Vertex render mode if no texture
			shader.setFloat("RenderTexture", 0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, shape.indices.size(), GL_UNSIGNED_INT, (void *)(6 * shape.indices_index * sizeof(GLuint)));
		glBindVertexArray(0);
	}
}
/// ---



/// Getters
const Object &OBJ::getObjectData() const {
	return obj;
}
/// ---