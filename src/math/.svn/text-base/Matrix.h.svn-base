#pragma once

#include "Vector.h"

#include <cmath>
#include <iostream>

namespace math
{
template <class T>
class Matrix
{
public:
	/**
	 * Initialized to identity matrix.
	 */
	Matrix();
	Matrix(const Matrix<T>& m);
	Matrix(T aa, T ab, T ac, T ad,
	       T ba, T bb, T bc, T bd,
	       T ca, T cb, T cc, T cd,
	       T da, T db, T dc, T dd);

	const Matrix<T>& operator=(const Matrix<T>& m);

	T& operator( )(int i) { return _data[i]; }
	T operator() (int i) const { return _data[i]; }

	Matrix<T> operator* (const Matrix<T> &other);

	void zero (void);

	/**
	 * Return the element in the ith row, jth column.
	 */
	T& operator() (int i, int j) { return _data[j + i * 4]; }
	T operator() (int i, int j) const { return _data[j + i * 4]; }

	Vector<T, 4> row (int i) const;
	Vector<T, 4> col (int j) const;

	/**
	 * Return the internal array.
	 * The data is stored in column major form,
	 * just the way OpenGL likes it.
	 */
	T *data (void) { return _data; }
	const T *data (void) const { return _data; }

	void setData (T *data);

private:
	T _data[16];

};

// 00 04 08 12
// 01 05 09 13
// 02 06 10 14
// 03 07 11 15

template <class T>
Matrix<T>::Matrix()
{
	for (int i = 0; i < 16; ++i)
	{
		_data[i] = 0;
	}

	_data[0] = 1;
	_data[5] = 1;
	_data[10] = 1;
	_data[15] = 1;
}

template <class T>
void Matrix<T>::zero (void)
{
	for (size_t i = 0; i < 16; ++i)
	{
		_data[i] = 0;
	}
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& m)
{
	operator=(m);
}

template <class T>
Matrix<T>::Matrix(T aa, T ab, T ac, T ad,
                  T ba, T bb, T bc, T bd,
                  T ca, T cb, T cc, T cd,
                  T da, T db, T dc, T dd)
{
	_data[ 0] = aa; _data[ 4] = ab; _data[ 8] = ac; _data[12] = ad;
	_data[ 1] = ba; _data[ 5] = bb; _data[ 9] = bc; _data[13] = bd;
	_data[ 2] = ca; _data[ 6] = cb; _data[10] = cc; _data[14] = cd;
	_data[ 3] = da; _data[ 7] = db; _data[11] = dc; _data[15] = dd;
}

template <class T>
const Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
	for (int i = 0; i < 16; ++i)
	{
		_data[i] = m._data[i];
	}

	return m;
}

template <class T>
Matrix <T> Matrix<T>::operator* (const Matrix <T> &other)
{
	Matrix <T> result;

	for (size_t r = 0; r < 4; ++r)
	{
		for (size_t c = 0; c < 4; ++c)
		{
			for (size_t i = 0; i < 4; ++i)
			{
				result(r, c) += (*this)(r, i) * other(i, c);
			}
		}
	}

	return result;
}

template <class T>
Vector<T, 4> Matrix<T>::row(int i) const
{
	Vector<T, 4> v;

	for (int j = 0; j < 4; ++j)
	{
		v[j] = operator()(i, j);
	}

	return v;
}

template <class T>
Vector<T, 4> Matrix<T>::col(int j) const
{
	Vector<T, 4> v;

	for (int i = 0; i < 4; ++i)
	{
		v[i] = operator()(i, j);
	}

	return v;
}

template <class T>
Matrix<T> rotateX(T angle)
{
	double theta = angle.value();

	Matrix<T> R(1, 0, 0, 0,
	            0, cos(theta), -1 * sin(theta), 0,
	            0, sin(theta), cos(theta), 0,
	            0, 0, 0, 1);

	return R;
}

template <class T>
Matrix<T> rotateY(T angle)
{
	double theta = angle.value();

	Matrix<T> R(cos(theta), 0, sin(theta), 0,
	            0, 1, 0, 0,
	            -1 * sin(theta), 0, cos(theta), 0,
	            0, 0, 0, 1);

	return R;
}

template <class T>
Matrix<T> rotateZ(T angle)
{
	double theta = angle.value();

	Matrix<T> R(cos(theta), -1 * sin(theta), 0, 0,
	            sin(theta), cos(theta), 0, 0,
	            0, 0, 1, 0,
	            0, 0, 0, 1);

	return R;
}

template <class T>
Matrix<T> rotate(T angle, const Vector<T, 4>& p1, const Vector<T, 4>& p2)
{
	// http://www.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html

	Matrix<T> R;
	//Vector<T, 4> n = normalize(p2 - p1);

	T a = p1.x(), b = p1.y(), c = p1.z();
	T d = p2.x(), e = p2.y(), f = p2.z();
	//T u = n.x(), v = n.y(), w = n.z();
	T u = d - a, v = e - b, w = f - c;
	T u2 = u * u, v2 = v * v, w2 = w * w;
	T L2 = u2 + v2 + w2, L = sqrt(L2);

	T theta = angle;

#if 0
	R(0) = u2 + (v2 + w2) * cos(theta);
	R(1) = u * v * (1 - cos(theta)) + w * sin(theta);
	R(2) = u * w * (1 - cos(theta)) - v * sin(theta);
	R(3) = 0;

	R(4) = u * v * (1 - cos(theta)) - w * sin(theta);
	R(5) = v2 + (u2 + w2) * cos(theta);
	R(6) = v * w * (1 - cos(theta)) + u * sin(theta);
	R(7) = 0;

	R(8)  = u * w * (1 - cos(theta)) + v * sin(theta);
	R(9)  = v * w * (1 - cos(theta)) - u * sin(theta);
	R(10) = w2 + (u2 + v2) * cos(theta);
	R(11) = 0;

	R(12) = a * (v2 + w2) - u * (b * v + c * w) + (u * (b * v + c * w) - a * (v2 + w2)) * cos(theta) + (b * w - c * v) * sin(theta);
	R(13) = b * (u2 + w2) - v * (a * u + c * w) + (v * (a * u + c * w) - b * (u2 + w2)) * cos(theta) + (c * u - a * w) * sin(theta);
	R(14) = c * (u2 + v2) - w * (a * u + b * v) + (w * (a * u + b * v) - c * (u2 + v2)) * cos(theta) + (a * v - b * u) * sin(theta);
	R(15) = 1;
#endif

	R(0) = (u2 + (v2 + w2) * cos(theta)) / L2;
	R(1) = (u * v * (1 - cos(theta)) + w * L * sin(theta)) / L2;
	R(2) = (u * w * (1 - cos(theta)) - v * L * sin(theta)) / L2;
	R(3) = 0;

	R(4) = (u * v * (1 - cos(theta)) - w * L * sin(theta)) / L2;
	R(5) = (v2 + (u2 + w2) * cos(theta)) / L2;
	R(6) = (v * w * (1 - cos(theta)) + u * L * sin(theta)) / L2;
	R(7) = 0;

	R(8)  = (u * w * (1 - cos(theta)) + v * L * sin(theta)) / L2;
	R(9)  = (v * w * (1 - cos(theta)) - u * L * sin(theta)) / L2;
	R(10) = (w2 + (u2 + v2) * cos(theta)) / L2;
	R(11) = 0;

	R(12) = (a * (v2 + w2) - u * (b * v + c * w) + (u * (b * v + c * w) - a * (v2 + w2)) * cos(theta) + (b * w - c * v) * L * sin(theta)) / L2;
	R(13) = (b * (u2 + w2) - v * (a * u + c * w) + (v * (a * u + c * w) - b * (u2 + w2)) * cos(theta) + (c * u - a * w) * L * sin(theta)) / L2;
	R(14) = (c * (u2 + v2) - w * (a * u + b * v) + (w * (a * u + b * v) - c * (u2 + v2)) * cos(theta) + (a * v - b * u) * L * sin(theta)) / L2;
	R(15) = 1;

	return R;

#if 0
	Vector<T, 4> n = normalize(axis);
	T d = sqrt(n.y() * n.y() + n.z() * n.z());

	T thetaX = T(atan2(n.z(), n.y()));
	T thetaY = T(atan2(n.x(), n.z()));

	return rotateX<T>(-1 * thetaX) *
	       rotateY<T>(-1 * thetaY) *
	       rotateZ<T>(angle) *
	       rotateY<T>(thetaY) *
	       rotateX<T>(thetaX);
#endif
#if 0
	Matrix Rxn(1, 0, 0, 0,
	           0, n.z()/d, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1);
	Matrix Rxn(1, 0, 0, 0,
	           0, 1, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1);
	Matrix Rxn(1, 0, 0, 0,
	           0, 1, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1);

	Matrix Rxn(1, 0, 0, 0,
	           0, 1, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1);

	Matrix Rxn(1, 0, 0, 0,
	           0, 1, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1);
#endif
}

template <class T>
Matrix<T> rotate(T angle, const Vector<T, 4>& vector)
{
	return rotate(angle, Vector<T, 4>(0, 0, 0), vector);
}

template <class T>
Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B)
{
	Matrix<T> C;

	for (int j = 0; j < 4; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			C(i, j) = A.row(i).dot(B.col(j));
		}
	}

	return C;
}

template <class T>
Vector<T, 4> operator*(const Matrix<T>& M, const Vector<T, 4>& v)
{
	Vector<T, 4> result;

	for (int i = 0; i < 4; ++i)
	{
		result[i] = dot (M.row(i), v);
	}

	return result;
}



template <class T>
bool operator==(const Matrix<T>& A, const Matrix<T>& B)
{
	bool result = true;

	for (int j = 0; j < 4; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			result &= A(i, j) == B(i, j);
		}
	}

	return result;
}

template <class T>
bool operator!=(const Matrix<T>& A, const Matrix<T>& B)
{
	return !(A == B);
}

template <class T>
void Matrix<T>::setData (T *data)
{
	for (int i = 0; i < 16; ++i)
	{
		_data[i] = data[i];
	}
}

template <class T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m)
{
	out << std::endl << "[";

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			out << m(i, j);
			
			if (i != 3 || j != 3)
			{
				out << ", ";
			}
		}

		if (i != 3)
		{
			out << std::endl << ' ';
		}
	}

	out << "]" << std::endl;

	return out;
}

typedef Matrix<float> Matrixf;
typedef Matrix<double> Matrixd;

}
