#include "matrix4x4.h"
#include <string.h>
#include <math.h>

Matrix4x4::Matrix4x4(Matrix4x4& A)
{
	memcpy(m_d, A.m_d, sizeof(m_d));
}

Matrix4x4& Matrix4x4::operator =(Matrix4x4& A)
{
	memcpy(m_d, A.m_d, sizeof(m_d));
	return(*this);
}

void Matrix4x4::operator =(const Matrix4x4& A)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] = A[i][j];
}

void Matrix4x4::zero()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] = 0.0;
}

void Matrix4x4::identity()
{
	zero();
	for (int i = 0; i < 4; i++)
		m_d[i][i] = 1.0;
}

Matrix4x4 Matrix4x4::operator +(const Matrix4x4 &A)
{
	Matrix4x4 M;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = m_d[i][j] + A[i][j];

	return(M);
}

Matrix4x4 Matrix4x4::operator +()
{
	return(*this);
}

void Matrix4x4::operator +=(const Matrix4x4 &A)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] += A[i][j];
}

Matrix4x4 Matrix4x4::operator -(const Matrix4x4 &A)
{
	Matrix4x4 M;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = m_d[i][j] - A[i][j];

	return(M);
}

Matrix4x4 Matrix4x4::operator -()
{
	Matrix4x4 M;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = -m_d[i][j];

	return(M);
}

void Matrix4x4::operator -=(const Matrix4x4 &A)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] -= A[i][j];
}

Matrix4x4 Matrix4x4::operator *(const Matrix4x4 &A)
{
	Matrix4x4 M;
	double ab;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ab = 0.0;
			for (int k = 0; k < 4; k++)
				ab += m_d[i][k] * A[k][j];
			M[i][j] = ab;
		}
	}
	return(M);
}

void Matrix4x4::operator *=(const Matrix4x4 &A)
{
	double mm[4][4];
	double ab;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ab = 0.0;
			for (int k = 0; k < 4; k++)
				ab += m_d[i][k] * A[k][j];
			mm[i][j] = ab;
		}
	}
	memcpy(m_d, mm, sizeof(m_d));
}

Matrix4x4 Matrix4x4::operator *(double k)
{
	Matrix4x4 M;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = m_d[i][j] * k;

	return(M);
}

void Matrix4x4::operator *=(double k)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] *= k;
}

void Matrix4x4::neg()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] = -m_d[i][j];
}

void Matrix4x4::abs()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] = fabs(m_d[i][j]);
}

void Matrix4x4::transpose()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			double t = m_d[i][j];
			m_d[i][j] = m_d[j][i];
			m_d[j][i] = t;
		}
}

static inline double det2x2(double a, double b,
	double c, double d)
{
	return((a*d) - (b*c));
}


static double det3x3(double a1, double a2, double a3,
	double b1, double b2, double b3,
	double c1, double c2, double c3)
{
	return(a1*det2x2(b2, b3, c2, c3) -
		b1*det2x2(a2, a3, c2, c3) +
		c1*det2x2(a2, a3, b2, b3)
		);
}

double Matrix4x4::det()
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = m_d[0][0];
	b1 = m_d[1][0];
	c1 = m_d[2][0];
	d1 = m_d[3][0];
	a2 = m_d[0][1];
	b2 = m_d[1][1];
	c2 = m_d[2][1];
	d2 = m_d[3][1];
	a3 = m_d[0][2];
	b3 = m_d[1][2];
	c3 = m_d[2][2];
	d3 = m_d[3][2];
	a4 = m_d[0][3];
	b4 = m_d[1][3];
	c4 = m_d[2][3];
	d4 = m_d[3][3];
	return(a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4) -
		b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4) +
		c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4) -
		d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4)
		);
}

void Matrix4x4::adjoint()
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = m_d[0][0];
	b1 = m_d[1][0];
	c1 = m_d[2][0];
	d1 = m_d[3][0];
	a2 = m_d[0][1];
	b2 = m_d[1][1];
	c2 = m_d[2][1];
	d2 = m_d[3][1];
	a3 = m_d[0][2];
	b3 = m_d[1][2];
	c3 = m_d[2][2];
	d3 = m_d[3][2];
	a4 = m_d[0][3];
	b4 = m_d[1][3];
	c4 = m_d[2][3];
	d4 = m_d[3][3];
	m_d[0][0] = det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
	m_d[0][1] = -det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
	m_d[0][2] = det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
	m_d[0][3] = -det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
	m_d[1][0] = -det3x3(b1, b3, b4, c1, c3, c4, d1, d3, d4);
	m_d[1][1] = det3x3(a1, a3, a4, c1, c3, c4, d1, d3, d4);
	m_d[1][2] = -det3x3(a1, a3, a4, b1, b3, b4, d1, d3, d4);
	m_d[1][3] = det3x3(a1, a3, a4, b1, b3, b4, c1, c3, c4);
	m_d[2][0] = det3x3(b1, b2, b4, c1, c2, c4, d1, d2, d4);
	m_d[2][1] = -det3x3(a1, a2, a4, c1, c2, c4, d1, d2, d4);
	m_d[2][2] = det3x3(a1, a2, a4, b1, b2, b4, d1, d2, d4);
	m_d[2][3] = -det3x3(a1, a2, a4, b1, b2, b4, c1, c2, c4);
	m_d[3][0] = -det3x3(b1, b2, b3, c1, c2, c3, d1, d2, d3);
	m_d[3][1] = det3x3(a1, a2, a3, c1, c2, c3, d1, d2, d3);
	m_d[3][2] = -det3x3(a1, a2, a3, b1, b2, b3, d1, d2, d3);
	m_d[3][3] = det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
}

void Matrix4x4::copy(const Matrix4x4 &A)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_d[i][j] = A[i][j];
}

bool Matrix4x4::inv()
{
	double determinant = det();

	if (determinant < EPSILON)
		return false;

	Matrix4x4 A(*this);

	A.adjoint();
	A *= determinant;

	copy(A);

	return true;
}

void Matrix4x4::loadTranslate(double tx, double ty, double tz)
{
	identity();
	m_d[0][3] = tx;
	m_d[1][3] = ty;
	m_d[2][3] = tz;
}

void Matrix4x4::loadScale(double sx, double sy, double sz)
{
	identity();
	m_d[0][0] = sx;
	m_d[1][1] = sy;
	m_d[2][2] = sz;
}

void Matrix4x4::loadRotateX(double phi)
{
	double sinPhi = sin(phi);
	double cosPhi = cos(phi);

	zero();
	m_d[0][0] = 1.0;
	m_d[1][1] = cosPhi;
	m_d[2][2] = cosPhi;
	m_d[1][2] = -sinPhi;
	m_d[2][1] = sinPhi;
	m_d[3][3] = 1.0;
}

void Matrix4x4::loadRotateY(double phi)
{
	double sinPhi = sin(phi);
	double cosPhi = cos(phi);

	zero();
	m_d[0][0] = cosPhi;
	m_d[1][1] = 1.0;
	m_d[2][2] = cosPhi;
	m_d[0][2] = sinPhi;
	m_d[2][0] = -sinPhi;
	m_d[3][3] = 1.0;
}

void Matrix4x4::loadRotateZ(double phi)
{
	double sinPhi = sin(phi);
	double cosPhi = cos(phi);

	zero();
	m_d[0][0] = cosPhi;
	m_d[1][1] = cosPhi;
	m_d[2][2] = 1.0;
	m_d[0][1] = -sinPhi;
	m_d[1][0] = sinPhi;
	m_d[3][3] = 1.0;
}

Vector3D Matrix4x4::operator *(const Vector3D &A)
{
	Vector3D v;

	v.m_x = m_d[0][0] * A.m_x + m_d[0][1] * A.m_y + m_d[0][2] * A.m_z + m_d[0][3];
	v.m_y = m_d[1][0] * A.m_x + m_d[1][1] * A.m_y + m_d[1][2] * A.m_z + m_d[1][3];
	v.m_z = m_d[2][0] * A.m_x + m_d[2][1] * A.m_y + m_d[2][2] * A.m_z + m_d[2][3];

	return v;
}

Vector3D Matrix4x4::Transform(const Vector3D &A)
{
	Vector3D v;

	double x = A.m_x;
	double y = A.m_y;
	double z = A.m_z;

	v.m_x = m_d[0][0] * x + m_d[0][1] * y + m_d[0][2] * z + m_d[0][3];
	v.m_y = m_d[1][0] * x + m_d[1][1] * y + m_d[1][2] * z + m_d[1][3];
	v.m_z = m_d[2][0] * x + m_d[2][1] * y + m_d[2][2] * z + m_d[2][3];

	return(v);
}

Vector4D Matrix4x4::operator *(const Vector4D &A)
{
	Vector4D v;

	v.m_x = m_d[0][0] * A.m_x + m_d[0][1] * A.m_y + m_d[0][2] * A.m_z + m_d[0][3] * A.m_w;
	v.m_y = m_d[1][0] * A.m_x + m_d[1][1] * A.m_y + m_d[1][2] * A.m_z + m_d[1][3] * A.m_w;
	v.m_z = m_d[2][0] * A.m_x + m_d[2][1] * A.m_y + m_d[2][2] * A.m_z + m_d[2][3] * A.m_w;
	v.m_w = m_d[3][0] * A.m_x + m_d[3][1] * A.m_y + m_d[3][2] * A.m_z + m_d[3][3] * A.m_w;

	return v;
}

Vector4D Matrix4x4::Transform(const Vector4D &A)
{
	Vector4D v;

	double x = A.m_x;
	double y = A.m_y;
	double z = A.m_z;
	double w = A.m_w;

	v.m_x = m_d[0][0] * x + m_d[0][1] * y + m_d[0][2] * z + m_d[0][3] * w;
	v.m_y = m_d[1][0] * x + m_d[1][1] * y + m_d[1][2] * z + m_d[1][3] * w;
	v.m_z = m_d[2][0] * x + m_d[2][1] * y + m_d[2][2] * z + m_d[2][3] * w;
	v.m_w = m_d[3][0] * x + m_d[3][1] * y + m_d[3][2] * z + m_d[3][3] * w;

	return(v);
}

Vector3D Matrix4x4::Translate(const Vector3D &A)
{
	Vector3D v(m_d[0][3], m_d[1][3], m_d[2][3]);

	v += A;

	return v;
}


Vector4D Matrix4x4::Translate(const Vector4D &A)
{
	Vector4D v(m_d[0][3], m_d[1][3], m_d[2][3], 0.0);

	v += A;

	return v;
}

bool Matrix4x4::cmp(Matrix4x4 &A)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (fabs(m_d[i][j] - A[i][j]) < EPSILON)
				return false;

	return true;
}

bool Matrix4x4::operator ==(Matrix4x4 &A)
{
	return cmp(A);
}

bool Matrix4x4::operator !=(Matrix4x4 &A)
{
	return (!cmp(A));
}

void Matrix4x4::loadRotateAboutAxisThroughOrigin(const Vector3D &V, double phi)
{
	zero();
	m_d[3][3] = 1.0;

	double cos_phi = cos(phi);
	double sin_phi = sin(phi);
	double t = 1.0 - cos_phi;

	Vector3D VN(V);
	VN.Normalize();

	m_d[0][0] = VN.X()*VN.X() * t + cos_phi;
	m_d[0][1] = VN.X()*VN.Y() * t - VN.Z()*sin_phi;
	m_d[0][2] = VN.X()*VN.Z() * t + VN.Y()*sin_phi;

	m_d[1][0] = VN.X()*VN.Y() * t + VN.Z()*sin_phi;
	m_d[1][1] = VN.Y()*VN.Y() * t + cos_phi;
	m_d[1][2] = VN.Y()*VN.Z() * t - VN.X()*sin_phi;

	m_d[2][0] = VN.X()*VN.Z() * t - VN.Y()*sin_phi;
	m_d[2][1] = VN.Y()*VN.Z() * t + VN.X()*sin_phi;
	m_d[2][2] = VN.Z()*VN.Z() * t + cos_phi;
}

void Matrix4x4::loadRotateAboutAxisThroughOrigin(const Vector4D &V, double phi)
{
	loadRotateAboutAxisThroughOrigin(Vector3D(V), phi);
}

void Matrix4x4::loadRotate(const Vector3D &point, const Vector3D &axis, double phi)
{
	Vector3D axis_norm(axis);

	double a = point.m_x;
	double b = point.m_y;
	double c = point.m_z;

	axis_norm.Normalize();
	double u = axis_norm.X();
	double v = axis_norm.Y();
	double w = axis_norm.Z();

	double cos_phi = cos(phi);
	double sin_phi = sin(phi);
	double t = 1.0 - cos_phi;

	identity();

	m_d[0][0] = u*u + (v*v + w*w)*cos_phi;
	m_d[0][1] = u*v*t - w*sin_phi;
	m_d[0][2] = u*w*t + v*sin_phi;
	m_d[0][3] = (a*(v*v + w*w) - u*(b*v + c*w))*t + (b*w - c*v)*sin_phi;

	m_d[1][0] = u*v*t + w*sin_phi;
	m_d[1][1] = v*v + (u*u + w*w)*cos_phi;
	m_d[1][2] = v*w*t - u*sin_phi;
	m_d[1][3] = (b*(u*u + w*w) - v*(a*u + c*w))*t + (c*u - a*w)*sin_phi;

	m_d[2][0] = u*w*t - v*sin_phi;
	m_d[2][1] = v*w*t + u*sin_phi;
	m_d[2][2] = w*w + (u*u + v*v)*cos_phi;
	m_d[2][3] = (c*(u*u + v*v) - w*(a*u + b*v))*t + (a*v - b*u)*sin_phi;
}

void Matrix4x4::loadRotate(const Vector4D &point, const Vector4D &axis, double phi)
{
	loadRotate(Vector3D(point), Vector3D(axis), phi);
}
