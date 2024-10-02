#pragma once

# include <iostream>
# include <vector>
# include <sstream>
# include <fstream>
# include <string.h>

using namespace std;

typedef struct Vertex_s {
	double x;
	double y;
	double z;
} Vertex_t;

typedef struct Object_s {
	string name;
	vector<Vertex_t> vertices;
	vector<Vertex_t> normals;
	vector<Vertex_t> textures;
	vector<vector<int>> faces; // 3+ vertices
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
		const vector<int> &getFace(size_t index) const;

};