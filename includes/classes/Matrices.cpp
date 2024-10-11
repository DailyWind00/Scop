#include "Matrices.hpp"

//// AMatrice
/// Constructors & Destructors
AMatrice::AMatrice() {
	// Matrices are declared like this where V is a vector 3 :
	//     Vx Vy Vz Vw(=1)
	//      a  b  c  d
	//      e  f  g  h
	//      i  j  k  l
	//      m  n  o  p

	mat = mat4 { // Identity matrice in column format
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

AMatrice::~AMatrice() {
}
/// ---



/// Getters
const mat4 &AMatrice::getMatrice() const {
	return mat;
}
/// ---
//// ---






//// Translation Matrice
/// Constructors & Destructors
TranslationMatrice::TranslationMatrice(float x, float y, float z) {
	// Translation matrices look like this :
	//     1  0  0  0
	//     0  1  0  0
	//     0  0  1  0
	//     x  y  z  1

	mat[12] = x; // = m
	mat[13] = y; // = n
	mat[14] = z; // = o

	// mat is an identity matrice if no translation is set
}

TranslationMatrice::~TranslationMatrice() {
}
/// ---
//// ---






//// Rotation Matrice
/// Constructors & Destructors
RotationMatrice::RotationMatrice(ROTATION axis, float angle) {
	/// With c = cos(angle) and s = sin(angle)
	// Rotation matrices on pitch axis look like this :
	//     1  0   0  0
	//     0  c  -s  0
	//     0  s   c  0
	//     0  0   0  1

	// Rotation matrices on yaw axis look like this :
	//     c  0  s  0
	//     0  1  0  0
	//    -s  0  c  0
	//     0  0  0  1

	// Rotation matrices on roll axis look like this :
	//     c -s  0  0
	//     s  c  0  0
	//     0  0  1  0
	//     0  0  0  1
	/// ---

	if (!angle)
		return ;

	float cosAngle = cos(angle * M_PI / 180.0);
	float sinAngle = sin(angle * M_PI / 180.0);

	switch (axis) {
		case ROTATION::NONE:
			break;
		case ROTATION::PITCH: // x
			mat[5]  = cosAngle;    // = f
			mat[6]  = -sinAngle;   // = g
			mat[9]  = sinAngle;    // = j
			mat[10] = cosAngle;    // = k
			break;
		case ROTATION::YAW: // y
			mat[0]  = cosAngle;    // = a
			mat[2]  = sinAngle;    // = c
			mat[8]  = -sinAngle;   // = i
			mat[10] = cosAngle;    // = k
			break;
		case ROTATION::ROLL: // z
			mat[0]  = cosAngle;    // = a
			mat[1]  = -sinAngle;   // = b
			mat[4]  = sinAngle;    // = e
			mat[5]  = cosAngle;    // = f
			break;
	}

	// mat is an identity matrice if no rotation is set
}

RotationMatrice::~RotationMatrice() {
}
/// ---
//// ---






//// Scaling Matrice
/// Constructors & Destructors
ScalingMatrice::ScalingMatrice(float x, float y, float z) {
	// Scaling matrices look like this :
	//     1*x  0   0   0
	//      0  1*y  0   0
	//      0   0  1*z  0
	//      0   0   0   1

	mat[0] = x;  // = a
	mat[5] = y;  // = f
	mat[10] = z; // = k

	// mat is an identity matrice if no scaling is set
}

ScalingMatrice::~ScalingMatrice() {
}
/// ---
//// ---