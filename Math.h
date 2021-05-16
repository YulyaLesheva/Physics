#pragma once
#include "stdafx.h"

struct Math {

	float DEG2RAD(float& degree) {
		return (degree * (math::PI / 180));
	}

	void ROTATE(FPoint& vector, float& angleDegrees, FPoint& origin) {

		float x = vector.x - origin.x;
		float y = vector.y - origin.y;

		float cos = (float)math::cos(DEG2RAD(angleDegrees));
		float sin = (float)math::sin(DEG2RAD(angleDegrees));

		float xPrime = (x * cos) - (y * sin);
		float yPrime = (x * sin) + (y * cos);

		xPrime += origin.x;
		yPrime += origin.y;

		vector.x = xPrime;
		vector.y = yPrime;
	}

	FPoint ROTATE_RETURN(FPoint vector, float angleDegrees, FPoint origin) {

		float x = vector.x - origin.x;
		float y = vector.y - origin.y;

		float cos = (float)math::cos(DEG2RAD(angleDegrees));
		float sin = (float)math::sin(DEG2RAD(angleDegrees));

		float xPrime = (x * cos) - (y * sin);
		float yPrime = (x * sin) + (y * cos);

		xPrime += origin.x;
		yPrime += origin.y;

		vector.x = xPrime;
		vector.y = yPrime;

		return FPoint(xPrime, yPrime);
	}

	FPoint Cross(FPoint& a, float s) {
		return FPoint(s * a.y, -s * a.x);
	}
	FPoint Cross(float s, FPoint& a) {
		return FPoint(-s * a.y, s * a.x);
	}
	
	float  Cross(FPoint& a, FPoint& b) {
		return a.x * b.y - a.y * b.x;
	}

	float Dot(const FPoint& a, const FPoint& b) {
		return a.x * b.x + a.y * b.y;
	}

	float Clamp(float a, float low, float high) {
		return math::max(low, math::min(a, high));
	}
	
	template<typename T> void Swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}

};

struct Matrix22
{
	Matrix22() {}
	Matrix22(float angle)
	{
		float c = cosf(angle), s = sinf(angle);
		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	Matrix22(const FPoint& col1, const FPoint& col2):
	col1(col1), col2(col2){}
	
	Matrix22 Transpose() const {
		return Matrix22(FPoint(col1.x, col2.x), FPoint(col1.y, col2.y));
	}

	Matrix22 Invert() const {
		float a = col1.x, b = col2.x, c = col1.y, d = col2.y;
		Matrix22 B;
		float det = a * d - b * c;
		assert(det != 0.0f);
		det = 1.0f / det;
		B.col1.x =  det * d;	B.col2.x = -det * b;
		B.col1.y = -det * c;	B.col2.y =  det * a;
		return B;
	}

	//variables
	FPoint col1, col2;
};


//for 2D Vectors 
inline FPoint operator * (const Matrix22& A, const FPoint& v) {
	return FPoint(A.col1.x * v.x + A.col2.x * v.y, A.col1.y * v.x + A.col2.y * v.y);
}

inline FPoint Abs(const FPoint a) {
	return FPoint(fabsf(a.x), fabsf(a.y));
}

//for matrix 
inline Matrix22 operator + (const Matrix22& A, const Matrix22& B) {
	return Matrix22(A.col1 + B.col1, A.col2 + B.col2);
}

inline Matrix22 operator * (const Matrix22& A, const Matrix22& B){
	return Matrix22(A * B.col1, A * B.col2);
}

inline Matrix22 Abs(const Matrix22& A) {
	return Matrix22(Abs(A.col1), Abs(A.col2));
}

inline float Abs(float a)
{
	return a > 0.0f ? a : -a;
}