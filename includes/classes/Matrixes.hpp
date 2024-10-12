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
class Matrix {
	protected:
		mat4	mat;

	public :
		Matrix();
		virtual	~Matrix();

		Matrix operator*(const Matrix &rhs) const;

		// Getter

		const mat4 &getMatrix() const;
};

// Translation matrix class used for moving object, take 3 arguments at creation :
//     float x = x position multiplier      (default = 0)
//     float y = y position multiplier      (default = 0)
//     float z = z position multiplier      (default = 0)
class TranslationMatrix : public Matrix {
	public :
		TranslationMatrix(float x = 0, float y = 0, float z = 0);
		~TranslationMatrix();
};

// Scaling matrix class, take 3 arguments at creation :
//     ROTATION axis = rotation axis        (default = NONE) # see ROTATION enum in config.hpp
//     float angle   = angle in degrees     (default = 0)
class RotationMatrix : public Matrix {
	public :
		RotationMatrix(ROTATION axis = ROTATION::NONE, float angle = 0);
		~RotationMatrix();
};

// Scaling matrix class used for scaling object, take 3 arguments at creation :
//     float x = x axis size multiplier     (default = 1)
//     float y = y axis size multiplier     (default = 1)
//     float z = z axis size multiplier     (default = 1)
class ScalingMatrix : public Matrix {
	public :
		ScalingMatrix(float x = 1, float y = 1, float z = 1);
		~ScalingMatrix();
};