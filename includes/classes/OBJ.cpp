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
void	OBJ::parseOBJ(const string &file_name) {
	ifstream	object_file = openReadFile(file_name);
	string		line;
	int			i = 0;
	int			current_color = 0;

	while (getline(object_file, line)) {
		istringstream iss(line);
		string type;
		iss >> type;
		i++;

		if (type == "o") {
			iss >> obj.name;
			if (!iss)
				throw runtime_error("Error while parsing object name at line " + to_string(i));
		}
		else if (type == "v") {
			float x, y, z;
			iss >> x >> y >> z;
			obj.positions.push_back(x);
			obj.positions.push_back(y);
			obj.positions.push_back(z);
			if (!iss)
				throw runtime_error("Error while parsing vertices at line " + to_string(i));
		}
		else if (type == "vt") {
			float x, y;
			iss >> x >> y;
			obj.textures.push_back(x);
			obj.textures.push_back(y);
			if (!iss)
				throw runtime_error("Error while parsing textures at line " + to_string(i));
		}
		else if (type == "vn") {
			float x, y, z;
			iss >> x >> y >> z;
			obj.normals.push_back(x);
			obj.normals.push_back(y);
			obj.normals.push_back(z);
			if (!iss)
				throw runtime_error("Error while parsing normals at line " + to_string(i));
		}
		else if (type == "f") {
			string vertex;
			std::vector<Indices> faceIndices;
			while (iss >> vertex) {
				istringstream vss(vertex);
				string index;
				Indices values = {NO_INDEX, NO_INDEX, NO_INDEX};
				int idx = 0;
				while (getline(vss, index, '/')) {
					if (!index.empty())
						values[idx] = stoi(index) - 1;
					idx++;
				}
				faceIndices.push_back(values);
			}
			if (faceIndices.size() == 3) {
				obj.indices.insert(obj.indices.end(), faceIndices.begin(), faceIndices.end());
				obj.colors.push_back(DEFAULT_COLORS[current_color % 3]);
			}
			else if (faceIndices.size() == 4) {
				obj.indices.push_back(faceIndices[0]);
				obj.indices.push_back(faceIndices[1]);
				obj.indices.push_back(faceIndices[2]);
				obj.indices.push_back(faceIndices[0]);
				obj.indices.push_back(faceIndices[2]);
				obj.indices.push_back(faceIndices[3]);
				obj.colors.push_back(DEFAULT_COLORS[current_color % 3]);
			}
			else
				throw runtime_error("Error while parsing face at line " + to_string(i) + " : invalid number of vertices");
			current_color++;
		}
		else if (type == "mtllib") {
			string mtl_file_name;
			iss >> mtl_file_name;
			if (!iss)
				throw runtime_error("Error while parsing material file at line " + to_string(i));
			if (mtl_file_name.size() < 4 || mtl_file_name.substr(mtl_file_name.size() - 4) != ".mtl")
				throw runtime_error("Error while opening material file : invalid extension");

			mtl_file_name = file_name.substr(0, file_name.find_last_of("/\\") + 1) + mtl_file_name; // get the path of the mtl file

			try {
				printVerbose("Parsing material file : " + mtl_file_name);
				parseMTL(mtl_file_name);
				printVerbose("Material file parsed");
			}
			catch(const std::exception& e) {
				throw runtime_error("Error in " + mtl_file_name + " : " + e.what());
			}
		}
		else if (type == "usemtl") {
			string material_name;
			iss >> material_name;
			if (!iss)
				throw runtime_error("Error while parsing material name at line " + to_string(i));
			if (obj.materials.find(material_name) == obj.materials.end()) {
				cout << BOrange << "Warning : material " << material_name << " not found at line " << i << ResetColor << endl;
				continue;
			}
			// TODO : set the material
		}
	}
	if (obj.name.empty())
		obj.name = "Object Viewer";
	setObjectSize();
	setObjectCentroid();
	object_file.close();
}

void	OBJ::parseMTL(const string &object_file_path) {
	ifstream	object_file = openReadFile(object_file_path);
	string	line;
	int		i = 0;
	mtl_Data current_mtl;

	while (getline(object_file, line)) {
		istringstream iss(line);
		string type;
		iss >> type;
		i++;

		if (type == "newmtl") {
			if (!current_mtl.material.empty()) {
				obj.materials.insert(pair<string, mtl_Data>(current_mtl.material, current_mtl));
				current_mtl = {}; // reset the current material
			}
			string material_name;
			iss >> material_name;
			if (!iss)
				throw runtime_error("Error while parsing material name at line " + to_string(i));
			current_mtl.material = material_name;
		}
		else if (type == "map_Kd") {
			string texture_path;
			iss >> texture_path;
			if (!iss)
				throw runtime_error("Error while parsing texture path at line " + to_string(i));
			current_mtl.texture_path = object_file_path.substr(0, object_file_path.find_last_of("/\\") + 1) + texture_path; // get the path of the texture file
		}
	}
	if (!current_mtl.material.empty())
		obj.materials.insert(pair<string, mtl_Data>(current_mtl.material, current_mtl));
	object_file.close();
	printVerbose(to_string(obj.materials.size()) + " materials loaded");
	for (const auto &mtl : obj.materials)
		printVerbose("Material : " + mtl.first + " - Texture : " + mtl.second.texture_path);
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
    if (obj.positions.empty()) {
        obj.centroid = {0.0f, 0.0f, 0.0f};
        return;	
    }

	vec3 max_ = {0.0f, 0.0f, 0.0f};
	vec3 min_ = {0.0f, 0.0f, 0.0f};
    size_t numVertices = obj.positions.size() / 3;

    for (size_t i = 0; i < numVertices; ++i) {
        max_[0] = max(max_[0], obj.positions[3 * i]);
		max_[1] = max(max_[1], obj.positions[3 * i + 1]);
		max_[2] = max(max_[2], obj.positions[3 * i + 2]);

		min_[0] = min(min_[0], obj.positions[3 * i]);
		min_[1] = min(min_[1], obj.positions[3 * i + 1]);
		min_[2] = min(min_[2], obj.positions[3 * i + 2]);
    }

	obj.size = max(max_[0] - min_[0], max(max_[1] - min_[1], max_[2] - min_[2]));
}

void	OBJ::setObjectCentroid() {
    if (obj.positions.empty()) {
        obj.centroid = {0.0f, 0.0f, 0.0f};
        return;	
    }

    vec3 sum = {0.0f, 0.0f, 0.0f};
    size_t numVertices = obj.positions.size() / 3;

    for (size_t i = 0; i < numVertices; ++i) {
        sum[0] += obj.positions[3 * i];
        sum[1] += obj.positions[3 * i + 1];
        sum[2] += obj.positions[3 * i + 2];
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
    std::vector<Vertex> vertices;
	int i = 0;

	cout << obj.indices.size() << endl;
    for (const Indices& idx : obj.indices) {
        Vertex vertex;

        vertex.position = {obj.positions[3 * idx[0]], obj.positions[3 * idx[0] + 1], obj.positions[3 * idx[0] + 2]};
		vertex.color = {obj.colors[i/3], obj.colors[i/3], obj.colors[i/3]};
		
        if (idx[1] != NO_INDEX)
            vertex.texCoords = {obj.textures[2 * idx[1]], obj.textures[2 * idx[1] + 1]};
        else
            vertex.texCoords = {0.0f, 0.0f};

        if (idx[2] != NO_INDEX)
            vertex.normal = {obj.normals[3 * idx[2]], obj.normals[3 * idx[2] + 1], obj.normals[3 * idx[2] + 2]};
        else
            vertex.normal = {0.0f, 0.0f, 0.0f};

        vertices.push_back(vertex);
		i++;
    }

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Bind and set EBO
    std::vector<GLuint> elementIndices;
    for (size_t i = 0; i < obj.indices.size(); ++i) {
        elementIndices.push_back(static_cast<GLuint>(i));
    }
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
	glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
/// ---



/// Getters
const Object_Data	&OBJ::getObjectData() const {
	return obj;
}

const GLuint		&OBJ::getVAO() const {
	return VAO;
}

const GLuint		&OBJ::getVBO() const {
	return VBO;
}

const GLuint		&OBJ::getEBO() const {
	return EBO;
}

const GLuint		&OBJ::getTBO() const {
	return TBO;
}
/// ---