#include "OBJ.hpp"

/// Constructors & Destructors 
OBJ::OBJ(const string &file_name) {
	if (file_name.size() < 4 || file_name.substr(file_name.size() - 4) != ".obj")
		throw runtime_error("Error while opening object file : invalid extension");

	ifstream	object_file(file_name.c_str());
	if (!object_file.is_open() || object_file.bad())
		throw runtime_error("Error while opening object file : " + (string)strerror(errno));
	
	try {
		// this->obj = parseOBJ(object_file);
	}
	catch(const std::exception& e) {
		throw runtime_error("Error in " + file_name + " : " + e.what());
	}
	object_file.close();
}

OBJ::~OBJ() {
}
/// ---



/// Getters

// return the name of the object in the OBJ file (o <name>)
const string &OBJ::getObjectName() const {
	return this->obj.name;
}

// return the vertex at the given index (v <x> <y> <z>)
const Vertex_t &OBJ::getVertice(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.vertices[index];
}

// return the normal at the given index (vn <x> <y> <z>)
const Vertex_t &OBJ::getNormal(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.normals[index];
}

// return the texture at the given index (vt <x> <y> <z>)
const Vertex_t &OBJ::getTexture(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.textures[index];
}

// return the face at the given index (f <v1>/[vn1]/[vt1] <v2>/[vn2]/[vt2] <v3>/[vn3]/[vt3])
const vector<int> &OBJ::getFace(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.faces[index];
}
/// ---