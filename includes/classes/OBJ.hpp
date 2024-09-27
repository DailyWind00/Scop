#pragma once

# include <iostream>
# include <deque>
# include <sstream>

using namespace std;

typedef struct Vertex_s {
	double x;
	double y;
	double z;
} Vertex_t;

typedef struct Object_s {
	string name;
	deque<Vertex_t> vertices;
	deque<Vertex_t> normals;
	deque<Vertex_t> textures;
	deque<deque<int>> faces; // 3+ vertices
} Object_t;

class OBJ {
	private:
		Object_t obj;

	public:
		OBJ(const string &file_name);
		~OBJ();

		// Getters
		const string &getObjectName() const;
		const Vertex_t &getVertice(size_t index) const;
		const Vertex_t &getNormal(size_t index) const;
		const Vertex_t &getTexture(size_t index) const;
		const deque<int> &getFace(size_t index) const;

};