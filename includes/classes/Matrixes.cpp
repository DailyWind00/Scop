#include "Matrixes.hpp"

//// Amatrix
/// Constructors & Destructors
Matrix::Matrix() {
	// matrixes are declared like this where V is a vector 3 :
	//     Vx Vy Vz Vw(=1)
	//
	//      a  b  c  d
	//      e  f  g  h
	//      i  j  k  l
	//      m  n  o  p

	mat = mat4 { // Identity matrix in column format
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Matrix::Matrix(const Matrix &matrix) {
	(*this) = matrix;
}

// Create a Matrix class with the matrix passed in argument
// Can be used to get the features of the Matrix class on a matrix
Matrix::Matrix(mat4 matrix) {
	mat = matrix;
}

Matrix & Matrix::operator=(const Matrix &matrix) {
	this->mat = matrix.mat;
}

Matrix::~Matrix() {
}
/// ---

/// Operators
Matrix Matrix::operator*(const Matrix &rhs) const {
	Matrix result;

	for (int i = 0; i < 16; i += 4) {
		for (int j = 0; j < 4; j++) {
			result.mat[i + j] = 0;
			for (int k = 0; k < 4; k++)
				result.mat[i + j] += mat[i + k] * rhs.mat[k * 4 + j];
		}
	}

	return result;
}


/// Getters
const mat4 &Matrix::getMatrix() const {
	return mat;
}
/// ---
//// ---






//// Translation matrix
/// Constructors & Destructors
TranslationMatrix::TranslationMatrix(float x, float y, float z) {
	// Translation matrixes look like this :
	//     1  0  0  0
	//     0  1  0  0
	//     0  0  1  0
	//     x  y  z  1

	mat[12] = x; // = m
	mat[13] = y; // = n
	mat[14] = z; // = o

	// mat is an identity matrix if no translation is set
}

TranslationMatrix::~TranslationMatrix() {
}
/// ---
//// ---






//// Rotation matrix
/// Constructors & Destructors
RotationMatrix::RotationMatrix(ROTATION axis, float angle) {
	/// With c = cos(angle) and s = sin(angle)
	// Rotation matrixes on pitch axis look like this :
	//     1  0   0  0
	//     0  c  -s  0
	//     0  s   c  0
	//     0  0   0  1

	// Rotation matrixes on yaw axis look like this :
	//     c  0  s  0
	//     0  1  0  0
	//    -s  0  c  0
	//     0  0  0  1

	// Rotation matrixes on roll axis look like this :
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

	// mat is an identity matrix if no rotation or angle is set
}

RotationMatrix::~RotationMatrix() {
}
/// ---
//// ---






//// Scaling matrix
/// Constructors & Destructors
ScalingMatrix::ScalingMatrix(float x, float y, float z) {
	// Scaling matrixes look like this :
	//     1*x  0   0   0
	//      0  1*y  0   0
	//      0   0  1*z  0
	//      0   0   0   1

	mat[0] = x;  // = a
	mat[5] = y;  // = f
	mat[10] = z; // = k

	// mat is an identity matrix if no scaling is set
}

ScalingMatrix::~ScalingMatrix() {
}
/// ---
//// ---