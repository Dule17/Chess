#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#include <assert.h>
#include "vector4d.h"

using namespace std;
#define EPSILON 1.0e-10

enum ProjectionType { ORTHOGRAPHIC_PROJECTION, PERSPECTIVE_PROJECTION };

class Matrix4x4
{
public:
	double m_d[4][4];
public:
	Matrix4x4() {}
	Matrix4x4(Matrix4x4& A);
	Matrix4x4& operator=(Matrix4x4& A);

	void zero();
	void identity();

	void copy(const Matrix4x4& A);

	inline void set(int row, int col, double value);

	inline double* operator[](int index);
	inline const double* operator[](int index) const;

	void operator=(const Matrix4x4& A);
	Matrix4x4 operator+(const Matrix4x4& A);
	Matrix4x4 operator+();
	void operator+=(const Matrix4x4& A);
	Matrix4x4 operator-();
	Matrix4x4 operator-(const Matrix4x4& A);
	void operator-=(const Matrix4x4& A);
	Matrix4x4 operator*(const Matrix4x4& A);
	void operator*=(const Matrix4x4& A);
	Matrix4x4 operator*(double k);
	void operator*=(double k);

	void neg();
	void abs();
	void transpose();

	double det();
	void adjoint();
	bool inv();

	bool cmp(Matrix4x4 &A);
	bool operator ==(Matrix4x4 &A);
	bool operator !=(Matrix4x4 &A);

	void loadTranslate(double tx, double ty, double tz);
	void loadScale(double sx, double sy, double sz);
	void loadRotateX(double phi);
	void loadRotateY(double phi);
	void loadRotateZ(double phi);

	void loadRotateAboutAxisThroughOrigin(const Vector3D &V, double phi);
	void loadRotateAboutAxisThroughOrigin(const Vector4D &V, double phi);

	void loadRotate(const Vector3D &point, const Vector3D &axis, double phi);
	void loadRotate(const Vector4D &point, const Vector4D &axis, double phi);

	Vector3D operator*(const Vector3D &A);
	Vector3D Transform(const Vector3D &A);

	Vector4D operator*(const Vector4D &A);
	Vector4D Transform(const Vector4D &A);

	Vector3D Translate(const Vector3D &A);
	Vector4D Translate(const Vector4D &A);
};

inline const double* Matrix4x4::operator[](int index) const
{
	assert(index >= 0 && index<4);
	return((double*)&m_d[index]);
}

inline double* Matrix4x4::operator[](int index)
{
	assert(index >= 0 && index<4);
	return((double*)&m_d[index]);
}

inline void Matrix4x4::set(int row, int col, double value)
{
	assert(row >= 0 && row<4 && col >= 0 && col<4);
	m_d[row][col] = value;
}

#endif // MATRIX4X4_H
