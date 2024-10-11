#pragma once

# include <iostream>
# include <array>
# include "config.hpp"

using namespace std;

typedef array<float, 16> mat4; // using column format

// Default matrice class, generate an identity matrice at creation
class AMatrice {
	protected:
		mat4	mat;

	public :
		AMatrice();
		virtual	~AMatrice() = 0;
};

// Translation matrice class used for moving object, take 3 arguments at creation :
//     float x = new x position          (default = 1)
//     float y = new y position          (default = 1)
//     float z = new z position          (default = 1)
class TranslationMatrice : public AMatrice {
	public :
		TranslationMatrice(float x = 1, float y = 1, float z = 1);
		~TranslationMatrice();
};

// Scaling matrice class, take 3 arguments at creation :
//     ROTATION axis = rotation axis     (default = None)
//     float angle   = angle in degrees  (default = 0)
class RotationMatrice : public AMatrice {
	public :
		RotationMatrice(ROTATION axis = NONE, float angle = 0);
		~RotationMatrice();
};

// Scaling matrice class used for scaling object, take 3 arguments at creation :
//     float x = new scale on x axis     (default = 1)
//     float y = new scale on y axis     (default = 1)
//     float z = new scale on z axis     (default = 1)
class ScalingMatrice : public AMatrice {
	public :
		ScalingMatrice(float x = 1, float y = 1, float z = 1);
		~ScalingMatrice();
};