#pragma once

# include <iostream>
# include <array>
# include <cmath>
# include "config.hpp"

using namespace std;

enum class ROTATION {
	NONE,
	PITCH,      // x
	YAW,        // y
	ROLL        // z
};

typedef array<float, 16> mat4; // using column format

// Default matrix class, generate an identity matrix at creation
class Amatrix {
	protected:
		mat4	mat;

	public :
		Amatrix();
		virtual	~Amatrix() = 0;


		// Getter

		const mat4 &getmatrix() const;
};

// Translation matrix class used for moving object, take 3 arguments at creation :
//     float x = new x position          (default = 0)
//     float y = new y position          (default = 0)
//     float z = new z position          (default = 0)
class Translationmatrix : public Amatrix {
	public :
		Translationmatrix(float x = 0, float y = 0, float z = 0);
		~Translationmatrix();
};

// Scaling matrix class, take 3 arguments at creation :
//     ROTATION axis = rotation axis     (default = None) # see ROTATION enum in config.hpp
//     float angle   = angle in degrees  (default = 0)
class Rotationmatrix : public Amatrix {
	public :
		Rotationmatrix(ROTATION axis = ROTATION::NONE, float angle = 0);
		~Rotationmatrix();
};

// Scaling matrix class used for scaling object, take 3 arguments at creation :
//     float x = new scale on x axis     (default = 1)
//     float y = new scale on y axis     (default = 1)
//     float z = new scale on z axis     (default = 1)
class Scalingmatrix : public Amatrix {
	public :
		Scalingmatrix(float x = 1, float y = 1, float z = 1);
		~Scalingmatrix();
};