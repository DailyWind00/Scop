#include "OBJ.hpp"

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
			printVerbose("+ Shape \"" + obj.shapes.back().name + "\" is now using material " + obj.shapes.back().name);
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
	printVerbose(to_string(obj.materials.size()) + " materials loaded :");
	for (const Material &mtl : obj.materials)
		printVerbose("| Material : " + mtl.name + " - Texture : " + mtl.texture_path);
}