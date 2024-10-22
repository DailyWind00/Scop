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

	// debugPrintObjectData(); // to remove
}

OBJ::~OBJ() {
}
/// ---



/// Privates functions
void	OBJ::parseOBJ(const string &file_name) {
	string	line;
	int		i = 0;

	ifstream	object_file(file_name.c_str());
	if (!object_file.is_open() || object_file.bad())
		throw runtime_error("Error while opening object file : " + (string)strerror(errno));

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
			} else if (faceIndices.size() == 4) {
				obj.indices.push_back(faceIndices[0]);
				obj.indices.push_back(faceIndices[1]);
				obj.indices.push_back(faceIndices[2]);
				obj.indices.push_back(faceIndices[0]);
				obj.indices.push_back(faceIndices[2]);
				obj.indices.push_back(faceIndices[3]);
			}
		}
		else if (type == "mtllib") {
			string mtl_file_name;
			iss >> mtl_file_name;
			if (!iss)
				throw runtime_error("Error while parsing material file at line " + to_string(i));
			if (mtl_file_name.size() < 4 || mtl_file_name.substr(mtl_file_name.size() - 4) != ".mtl")
				throw runtime_error("Error while opening material file : invalid extension");

			mtl_file_name = file_name.substr(0, file_name.find_last_of("/\\") + 1) + mtl_file_name; // get the path of the mtl file

			ifstream	mtl_file(mtl_file_name.c_str());
			if (!mtl_file.is_open() || mtl_file.bad())
				throw runtime_error("Error while opening " + mtl_file_name + " : " + (string)strerror(errno));

			try {
				printVerbose("Parsing material file : " + mtl_file_name);
				parseMTL(mtl_file);
				printVerbose("Material file parsed");
			}
			catch(const std::exception& e) {
				throw runtime_error("Error in " + mtl_file_name + " : " + e.what());
			}
			mtl_file.close();
		}
		else if (type == "usemtl") {
			string material_name;
			iss >> material_name;
			if (!iss)
				throw runtime_error("Error while parsing material name at line " + to_string(i));
			obj.material_names.push_back(material_name);
		}
	}
	if (obj.name.empty())
		obj.name = "Object Viewer";
	setObjectSize();
	setObjectCentroid();
	object_file.close();
}

void	OBJ::parseMTL(ifstream &object_file) {
	string	line;
	int		i = 0;

	while (getline(object_file, line)) {
		istringstream iss(line);
		string type;
		iss >> type;
		i++;

		if (type == "newmtl") {
			string material_name;
			iss >> material_name;
			if (!iss)
				throw runtime_error("Error while parsing material name at line " + to_string(i));
			obj.material_names.push_back(material_name);
		}
		else if (type == "Ka") {
			float r, g, b;
			iss >> r >> g >> b;
			obj.ambient_colors.push_back(r);
			obj.ambient_colors.push_back(g);
			obj.ambient_colors.push_back(b);
			if (!iss)
				throw runtime_error("Error while parsing ambient color at line " + to_string(i));
		}
		else if (type == "Kd") {
			float r, g, b;
			iss >> r >> g >> b;
			obj.diffuse_colors.push_back(r);
			obj.diffuse_colors.push_back(g);
			obj.diffuse_colors.push_back(b);
			if (!iss)
				throw runtime_error("Error while parsing diffuse color at line " + to_string(i));
		}
		else if (type == "Ks") {
			float r, g, b;
			iss >> r >> g >> b;
			obj.specular_colors.push_back(r);
			obj.specular_colors.push_back(g);
			obj.specular_colors.push_back(b);
			if (!iss)
				throw runtime_error("Error while parsing specular color at line " + to_string(i));
		}
		else if (type == "Ns") {
			float shininess;
			iss >> shininess;
			obj.shininess.push_back(shininess);
			if (!iss)
				throw runtime_error("Error while parsing shininess at line " + to_string(i));
		}
	}
}

void	OBJ::addTexture(const string &texture_path) {
	int width, height, nrChannels;
	unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
		throw runtime_error("Failed to load texture : " + texture_path);

	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D_ARRAY, TBO);
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

void	OBJ::debugPrintObjectData() const {
	cout << "Object name : " << obj.name << endl;
	cout << "Object size : " << obj.size << endl;
	cout << "Object center : " << obj.centroid[0] << " " << obj.centroid[1] << " " << obj.centroid[2] << endl;
	for (const auto &position : obj.positions) {
		cout << "Position : " << position << endl;
	}
	for (const auto &color : obj.colors) {
		cout << "Color : " << color << endl;
	}
	for (const auto &texture : obj.textures) {
		cout << "Texture : " << texture << endl;
	}
	for (const auto &normal : obj.normals) {
		cout << "Normal : " << normal << endl;
	}
	for (const auto &index : obj.indices) {
		cout << "Index : " << index[0] << " " << index[1] << " " << index[2] << endl;
	}
}
/// ---



/// Public functions
void	OBJ::setBuffers() {
	printVerbose("Setting GL buffers");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Interleave vertex attributes
    std::vector<Vertex> vertices;
    for (const Indices& idx : obj.indices) {
        Vertex vertex;
        vertex.position = {obj.positions[3 * idx[0]], obj.positions[3 * idx[0] + 1], obj.positions[3 * idx[0] + 2]};

        if (idx[1] != NO_INDEX)
            vertex.texCoords = {obj.textures[2 * idx[1]], obj.textures[2 * idx[1] + 1]};
        else
            vertex.texCoords = {0.0f, 0.0f};

        if (idx[2] != NO_INDEX)
            vertex.normal = {obj.normals[3 * idx[2]], obj.normals[3 * idx[2] + 1], obj.normals[3 * idx[2] + 2]};
        else
            vertex.normal = {0.0f, 0.0f, 0.0f};

        vertices.push_back(vertex);
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

	addTexture("red_square.png");

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