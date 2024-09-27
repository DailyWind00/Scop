#include "OBJ.hpp"

// Constructors & Destructors 
OBJ::OBJ(const string &file_name) {
	// parseOBJ(file_name);
	(void)file_name;
}

OBJ::~OBJ() {
}
// ---



// Getters
const string &OBJ::getObjectName() const {
	return this->obj.name;
}

const Vertex_t &OBJ::getVertice(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.vertices[index];
}

const Vertex_t &OBJ::getNormal(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.normals[index];
}

const Vertex_t &OBJ::getTexture(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.textures[index];
}

const deque<int> &OBJ::getFace(size_t index) const {
	if (index >= this->obj.vertices.size())
		throw out_of_range("Index out of range");
	return this->obj.faces[index];
}
// ---