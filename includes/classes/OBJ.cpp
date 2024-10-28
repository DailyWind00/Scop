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
void	OBJ::parseOBJ(const string &file_name) {
	ifstream	object_file = openReadFile(file_name);
	string		line;
	int			i = 0;

	while (getline(object_file, line)) {
		istringstream iss(line);
		string type;
		iss >> type;
		i++;

		if (type == "o") {
			if (!obj.name.empty())
				throw runtime_error("Error while parsing object name at line " + to_string(i) + " : object already defined");
			iss >> obj.name;
			if (!iss)
				throw runtime_error("Error while parsing object name at line " + to_string(i));
		}
		else if (type == "g") {
			Shape shape;
			iss >> shape.name;
			if (!iss)
				throw runtime_error("Error while parsing group name at line " + to_string(i));
			obj.shapes.push_back(shape);
		}
		else if (type == "v") {
			float x, y, z;
			iss >> x >> y >> z;
			obj.attributes.positions.push_back(x);
			obj.attributes.positions.push_back(y);
			obj.attributes.positions.push_back(z);
			if (!iss)
				throw runtime_error("Error while parsing vertices at line " + to_string(i));
		}
		else if (type == "vt") {
			float x, y;
			iss >> x >> y;
			obj.attributes.textures.push_back(x);
			obj.attributes.textures.push_back(y);
			if (!iss)
				throw runtime_error("Error while parsing textures at line " + to_string(i));
		}
		else if (type == "vn") {
			float x, y, z;
			iss >> x >> y >> z;
			obj.attributes.normals.push_back(x);
			obj.attributes.normals.push_back(y);
			obj.attributes.normals.push_back(z);
			if (!iss)
				throw runtime_error("Error while parsing normals at line " + to_string(i));
		}
		else if (type == "f") { // Faces ("f" : v/[vt]/[vn] v/[vt]/[vn] v/[vt]/[vn] ...)
			string vertex;
			std::vector<Indice> faceIndices;
			while (iss >> vertex) {
				istringstream vss(vertex);
				string index;
				Indice values = {NO_INDEX, NO_INDEX, NO_INDEX};
				int idx = 0;
				while (getline(vss, index, '/')) {
					if (!index.empty())
						values[idx] = stoi(index) - 1; // OBJ indices start at 1
					idx++;
				}
				faceIndices.push_back(values);
			}
			if (obj.shapes.empty()) { // Create a default group if no group is defined
				cout << BYellow << "Notice : no group defined at line " << i << ", creating a default group" << ResetColor << endl;
				Shape shape; shape.name = "default";
				obj.shapes.push_back(shape);
			}

			if (faceIndices.size() == 3) { // Triangle
				obj.shapes.back().indices.push_back(faceIndices[0]);
				obj.shapes.back().indices.push_back(faceIndices[1]);
				obj.shapes.back().indices.push_back(faceIndices[2]);
			}
			else if (faceIndices.size() == 4) { // Quad
				obj.shapes.back().indices.push_back(faceIndices[0]);
				obj.shapes.back().indices.push_back(faceIndices[1]);
				obj.shapes.back().indices.push_back(faceIndices[2]);
				obj.shapes.back().indices.push_back(faceIndices[0]);
				obj.shapes.back().indices.push_back(faceIndices[2]);
				obj.shapes.back().indices.push_back(faceIndices[3]);
			}
			else
				throw runtime_error("Error while parsing face at line " + to_string(i) + " : invalid number of vertices");
		}
		else if (type == "mtllib") {
			string mtl_path;
			iss >> mtl_path;
			if (!iss)
				throw runtime_error("Error while parsing material file at line " + to_string(i));
			if (mtl_path.size() < 4 || mtl_path.substr(mtl_path.size() - 4) != ".mtl")
				throw runtime_error("Error while opening material file : invalid extension");

			mtl_path = file_name.substr(0, file_name.find_last_of("/\\") + 1) + mtl_path; // get the path of the mtl file

			try {
				printVerbose("Parsing material file : " + mtl_path);
				parseMTL(mtl_path);
				printVerbose("Material file parsed");
			}
			catch(const std::exception& e) {
				throw runtime_error("Error in " + mtl_path + " : " + e.what());
			}
		}
		else if (type == "usemtl") {
			string material_name;
			iss >> material_name;
			if (!iss)
				throw runtime_error("Error while parsing material name at line " + to_string(i));

			auto it = obj.materials.begin();
			while (it->name != material_name && it != obj.materials.end())
				it++;

			if (it == obj.materials.end()) {
				cout << BOrange << "Warning : material \"" << material_name << "\" not found at line " << i << ResetColor << endl;
				continue;
			}

			if (obj.shapes.empty()) { // Create a default group if no group is defined
				cout << BYellow << "Notice : no group defined at line " << i << ", creating a default group" << ResetColor << endl;
				Shape shape; shape.name = "default";
				obj.shapes.push_back(shape);
			}

			obj.shapes.back().material = *it;
			printVerbose("Shape " + obj.shapes.back().name + " is now using material " + obj.shapes.back().name);
		}
	}
	if (obj.name.empty())
		obj.name = "Object Viewer";
	setObjectSize();
	setObjectCentroid();
	object_file.close();

	if (obj.shapes.empty()) {
		cout << BOrange << "Warning : no shapes found in the object file, calling destructor" << ResetColor << endl;
		this->~OBJ();
	}
}

void	OBJ::parseMTL(const string &object_file_path) {
	ifstream	object_file = openReadFile(object_file_path);
	string	line;
	int		i = 0;
	Material current_material;

	while (getline(object_file, line)) {
		istringstream iss(line);
		string type;
		iss >> type;
		i++;

		if (type == "newmtl") {
			if (!current_material.name.empty()) {
				obj.materials.push_back(current_material);
				current_material = {}; // reset the current material
			}
			iss >> current_material.name;
			if (!iss)
				throw runtime_error("Error while parsing material name at line " + to_string(i));
		}
		else if (type == "map_Kd") {
			string path;
			iss >> path;
			if (!iss)
				throw runtime_error("Error while parsing texture path at line " + to_string(i));
			path = object_file_path.substr(0, object_file_path.find_last_of("/\\") + 1) + path; // get the path of the texture file
			current_material.texture_path = path;
		}
	}
	obj.materials.push_back(current_material);
	object_file.close();
	printVerbose(to_string(obj.materials.size()) + " materials loaded");
	for (const Material &mtl : obj.materials)
		printVerbose("Material : " + mtl.name + " - Texture : " + mtl.texture_path);
}

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

	for (const Shape &shape : obj.shapes) {
		printVerbose("Shape : " + shape.name + " - Indices : " + to_string(shape.indices.size()));
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
		// if (obj.materials.find(shape.material) != obj.materials.end()) {
		// 	useTexture(obj.materials[shape.material].texture_path);
		// }
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