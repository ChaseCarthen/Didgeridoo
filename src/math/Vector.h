#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

//template<typename T, unsigned int R, unsigned int C> struct Matrix;

namespace math
{

/**
 * Represents a mathematical N-dimensional vector.
 */
template<typename T, unsigned int N>
struct Vector
{
	T v[N];

	/** Initializes all components to zero. */
	inline Vector<T,N>()
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	/** Initialize the first component, set all others to zero. */
	inline Vector<T,N>(const T x)
	{
		if (N >= 1)
		{
			v[0] = x;
		}
		for (unsigned int i = 1; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	/** Initialize the first two componenst, set all others to zero. */
	inline Vector<T,N>(const T x, const T y)
	{
		if (N >= 1)
		{
			v[0] = x;
		}
		if (N >= 2)
		{
			v[1] = y;
		}
		for (unsigned int i = 2; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	/** Initialize the first three components, set all others to zero. */
	inline Vector<T,N>(const T x, const T y, const T z)
	{
		if (N >= 1)
		{
			v[0] = x;
		}
		if (N >= 2)
		{
			v[1] = y;
		}
		if (N >= 3)
		{
			v[2] = z;
		}
		for (unsigned int i = 3; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	/** Initialize the first four components, set all others to zero. */
	inline Vector<T,N>(const T x, const T y, const T z, const T w)
	{
		if (N >= 1)
		{
			v[0] = x;
		}
		if (N >= 2)
		{
			v[1] = y;
		}
		if (N >= 3)
		{
			v[2] = z;
		}
		if (N >= 4)
		{	
			v[3] = w;
		}
		for (unsigned int i = 4; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	/** Costruct a vector from any other vector. */
	template <typename S, unsigned int M>
	inline Vector<T,N>(const Vector<S,M>& target)
	{
		for (unsigned int i = 0; i < N && i < M; ++i)
		{
			v[i] = target[i];
		}
		for (unsigned int i = M; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	/** Costruct a vector from any other vector. */
	template <typename S>
	inline Vector<T,N>(const S& target)
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			v[i] = target[i];
		}
	}

	inline Vector<T,N> operator+(const Vector<T,N>& right) const
	{
		Vector<T,N> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = v[i] + right.v[i];
		}
		return result;
	}

	inline void operator+=(const Vector<T,N>& right)
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			v[i] += right.v[i];
		}
	}

	inline Vector<T,N> operator-(const Vector<T,N>& right) const
	{
		Vector<T,N> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = v[i] - right.v[i];
		}
		return result;
	}

	inline void operator-=(const Vector<T,N>& right)
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			v[i] -= right.v[i];
		}
	}

	inline Vector<T,N> operator*(const T& right) const
	{
		Vector<T,N> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = v[i] * right;
		}
		return result;
	}

	inline Vector <T, N> operator* (const Vector <T, N> &right) const
	{
		Vector <T, N> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = v[i] * right[i];
		}

		return result;
	}

	inline void operator*=(const T& right)
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			v[i] *= right;
		}
	}

	inline Vector<T,N> operator/(const T& right) const
	{
		Vector<T,N> result;
		T tmp = 1 / right;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = v[i] * right;
		}
		return result;
	}

	inline Vector <T, N> operator/ (const Vector <T, N> &right) const
	{
		Vector <T, N> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = v[i] / right.v[i];
		}

		return result;
	}

	inline Vector<T,N> operator/=(const T& right)
	{
		T tmp = 1 / right;
		for (unsigned int i = 0; i < N; ++i)
		{
			v[i] *= tmp;
		}
		return *this;
	}

	inline void operator=(const Vector<T,N>& right)
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			v[i] = right.v[i];
		}
	}

	/** Assign any vector to any other vector instantiation. */
	template <typename S, unsigned int M>
	inline Vector<T,N>& operator=(const Vector<S,M>& target)
	{
		for (unsigned int i = 0; i < N && i < M; ++i)
		{
			v[i] = target[i];
		}

		for (unsigned int i = M; i < N; ++i)
		{
			v[i] = 0;
		}

		return *this;
	}

	/** Assign vector of same type but varying size to this vector. */
	template<unsigned int M>
	inline void operator=(const Vector<T,M>& right)
	{
		for (unsigned int i = 0; i < N & i < M; ++i)
		{
			v[i] = right.v[i];
		}
	}

	inline bool operator==(const Vector<T,N>& right) const
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			if (v[i] != right.v[i])
			{
				return false;
			}
		}
		return true;
	}

	inline bool operator!= (const Vector <T, N> &right) const
	{
		for (unsigned int i = 0; i < N; ++i)
		{
			if (v[i] != right.v[i])
			{
				return true;
			}
		}

		return false;
	}

	/*template<unsigned int C>
	inline Vector<T, C> operator*(const Matrix<T, N, C>& right) const
	{
		Vector<T, C> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = dot(right.col(i));
		}
		return result;
	}*/

	inline T& x()
	{
		return v[0];
	}

	inline T& y()
	{
		return v[1];
	}

	inline T& z()
	{
		return v[2];
	}

	inline T& w()
	{
		return v[3];
	}

	inline T x() const
	{
		return v[0];
	}

	inline T y() const
	{
		return v[1];
	}

	inline T z() const
	{
		return v[2];
	}

	inline T w() const
	{
		return v[3];
	}

	inline T& r()
	{
		return x();
	}

	inline T& g()
	{
		return y();
	}

	inline T& b()
	{
		return z();
	}

	inline T& a()
	{
		return w();
	}

	inline Vector <T, 2> xy (void)
	{
		Vector <T, 2> result;
		if (N >= 2)
		{
			result[0] = v[0];
			result[1] = v[1];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
		}

		return result;
	}

	inline Vector <T, 2> xz (void)
	{
		Vector <T, 2> result;
		if (N >= 3)
		{
			result[0] = v[0];
			result[1] = v[2];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
		}

		return result;
	}

	inline Vector <T, 2> yz (void)
	{
		Vector <T, 2> result;
		if (N >= 3)
		{
			result[0] = v[1];
			result[1] = v[2];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
		}

		return result;
	}

	inline Vector <T, 2> yx (void)
	{
		Vector <T, 2> result;
		if (N >= 2)
		{
			result[0] = v[1];
			result[1] = v[2];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
		}

		return result;
	}

	inline Vector <T, 2> zx (void)
	{
		Vector <T, 2> result;
		if (N >= 3)
		{
			result[0] = v[1];
			result[1] = v[2];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
		}

		return result;
	}

	inline Vector <T, 2> zy (void)
	{
		Vector <T, 2> result;
		if (N >= 3)
		{
			result[0] = v[2];
			result[1] = v[1];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
		}

		return result;
	}

	inline Vector <T, 3> xyz (void)
	{
		Vector <T, 3> result;
		if (N >= 3)
		{
			result[0] = v[0];
			result[1] = v[1];
			result[2] = v[2];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
			result[2] = 0;
		}

		return result;
	}

	inline Vector <T, 3> yxz (void)
	{
		Vector <T, 3> result;
		if (N >= 3)
		{
			result[0] = v[1];
			result[1] = v[0];
			result[2] = v[2];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
			result[2] = 0;
		}

		return result;
	}

	inline Vector <T, 3> zxy (void)
	{
		Vector <T, 3> result;
		if (N >= 3)
		{
			result[0] = v[2];
			result[1] = v[0];
			result[2] = v[1];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
			result[2] = 0;
		}

		return result;
	}

	inline Vector <T, 3> zyx (void)
	{
		Vector <T, 3> result;
		if (N >= 3)
		{
			result[0] = v[2];
			result[1] = v[1];
			result[2] = v[0];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
			result[2] = 0;
		}

		return result;
	}

	inline Vector <T, 4> xyzw (void)
	{
		Vector <T, 4> result;
		if (N >= 4)
		{
			result[0] = v[0];
			result[1] = v[1];
			result[2] = v[2];
			result[3] = v[3];
		}
		else
		{
			result[0] = 0;
			result[1] = 0;
			result[2] = 0;
			result[3] = 0;
		}

		return result;
	}

	inline T& operator[](int i)
	{
		return v[i];
	}

	inline T operator[](int i) const
	{
		return v[i];
	}

	/** Set all of the components to zero. */
	static inline Vector<T,N> zero()
	{
		Vector<T,N> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = 0;
		}
		return result;
	}

	/** Set all of the components to one. */
	static inline Vector<T,N> one()
	{
		Vector<T,N> result;
		for (unsigned int i = 0; i < N; ++i)
		{
			result.v[i] = 1;
		}
		return result;
	}
};

template<typename T, unsigned int N>
std::ostream& operator<<(std::ostream& out, const Vector<T, N>& p)
{
	out << '[' << p[0];

	for (unsigned int i = 1; i < N; ++i)
	{
		out << ", " << p[i];
	}

	out << ']';

	return out;
}

/** Calculate the magnitude of the vector. */
template <typename T, unsigned int N>
inline T length(const Vector <T, N> &vec) 
{
	T l = (T)0;
	for (unsigned int i = 0; i < N; ++i)
	{
		l += vec.v[i] * vec.v[i];
	}
	return (T) sqrt(l);
}

template <typename T, unsigned int N>
inline T length2(const Vector <T, N> &vec) 
{
	T l = (T)0;
	for (unsigned int i = 0; i < N; ++i)
	{
		l += vec.v[i] * vec.v[i];
	}
	return (T) l;
}

template <typename T, unsigned int N>
inline Vector <T, N> normalize (const Vector <T, N> &vec) 
{
	Vector <T, N> tmp (vec);
	return tmp /= length (tmp);
}

template <typename T, unsigned int N>
inline T dot(const Vector <T, N> &left, const Vector<T, N> &right) 
{
	T result = 0;
	for (unsigned int i = 0; i < N; ++i)
	{
		result += (left.v[i] * right.v[i]);
	}
	return result;
}

template <typename T>
inline Vector<T,3> cross(const Vector <T, 3> &left, const Vector<T,3> &right) 
{
	Vector<T,3>	result;
	result.x() = left.y() * right.z() - right.y() * left.z();
	result.y() = left.z() * right.x() - right.z() * left.x();
	result.z() = left.x() * right.y() - right.x() * left.y();
	return result;
}

/** Reflect the vector dir.  normal must be normalized before calling this function. */
template <typename T, unsigned int N>
inline Vector <T, N> reflect (const Vector <T, N> dir, const Vector <T, N> normal)
{
	return dir - (normal * ((T)2 * dot (dir, normal)));
}

/** Refract the vector dir.  normal and dir must be normalized before calling this function. */
template <typename T, unsigned int N>
inline bool refract (const Vector <T, N> dir, const Vector <T, N> normal, T ref, Vector <T, N> &new_dir)
{
#if 0
	T cos_theta_i = dot (normal, dir);
	T k = (T)1 - (ref * ref) * ((T)1 - cos_theta_i) * cos_theta_i;
	if (k < (T)0)
	{
		return false;
	}

	new_dir = normalize (dir * ref - normal * (ref * cos_theta_i + sqrt (k)));
	return true;
#endif

#if 1

	T view_projection = dot (dir, normal);
	T cos_theta_i = fabs (view_projection);
	T sin_theta_i = sqrt ((T)1 - cos_theta_i * cos_theta_i);
	T sin_theta_t = ref * sin_theta_i;
	if ((sin_theta_t * sin_theta_t) > 0.9999)
	{
		return false;
	}

	T cos_theta_t = sqrt ((T)1 - sin_theta_t * sin_theta_t);
	new_dir = normalize ((dir * ref) + (normal * -cos_theta_t));

	return true;
#endif
}

/** Component-wise min. */
template <typename T, unsigned int N>
inline Vector <T, N> vectorMin (const Vector <T, N> &left, const Vector <T, N> &right)
{
	Vector <T, N> result;
	for (unsigned int i = 0; i < N; ++i)
	{
		result.v[0] = std::min (left[i], right[i]);
	}

	return result;
}

/** Component-wise max. */
template <typename T, unsigned int N>
inline Vector <T, N> vectorMax (const Vector <T, N> &left, const Vector <T, N> &right)
{
	Vector <T, N> result;
	for (unsigned int i = 0; i < N; ++i)
	{
		result.v[0] = std::max (left[i], right[i]);
	}

	return result;
}

/** Perform fresnel reflectance.  This is similar to the refract function but it also calculates
    the percentage of light reflected and refracted on the surface. */
template <typename T, unsigned int N>
inline void fresnelRefract (const Vector <T, N> dir, const Vector <T, N> normal, T density1, T density2, Vector <T, N> &new_dir, T &kr, T &kt)
{
#if 0
	T ref = density1 / density2;
	if (refract (dir, normal, ref, new_dir))
	{
		T one_over_ref_squared = (T)1 / (ref * ref);
		T cos_theta_i = -dot (normal, dir);
		T tmp = (T)1 - ((T)1 - cos_theta_i * cos_theta_i) * one_over_ref_squared;
		T cos_theta_t = sqrt ((T)1 - ((T)1 - cos_theta_i * cos_theta_i) * one_over_ref_squared);
		T r_parallel = (ref * cos_theta_i - cos_theta_t) / (ref * cos_theta_i + cos_theta_t);
		T r_perpendicular = (cos_theta_i - ref * cos_theta_t) / (cos_theta_i + ref * cos_theta_t);
		kr = 0.5 * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);
		kt = (T)1 - kr;
	}
	else
	{
		kt = (T)0;
		kr = (T)1;
	}
#endif

#if 1
	if (refract (dir, normal, density1 / density2, new_dir))
	{
		T view_projection = dot (dir, normal);
		T cos_theta_i = fabs (view_projection);
		T sin_theta_i = sqrt ((T)1 - cos_theta_i * cos_theta_i);
		T sin_theta_t = (density1 / density2) * sin_theta_i;
		T cos_theta_t = sqrt ((T)1 - sin_theta_t * sin_theta_t);
		T perpendicular = (density2 * cos_theta_t - density1 * cos_theta_i) / (density2 * cos_theta_t + density1 * cos_theta_i);
		perpendicular *= perpendicular;
		T parallel = (density1 * cos_theta_t - density2 * cos_theta_i) / (density1 * cos_theta_t + density2 * cos_theta_i);
		kr = 0.5 * (parallel + perpendicular);
		kt = 1.0 - kr;
	}
	else
	{
		kt = (T)0;
		kr = (T)1;
	}
#endif
}

/**
  * Free form operator*.
  */
template <typename T, unsigned int N>
inline const Vector <T, N> operator* (const T &scalar, const Vector <T, N> &vec)
{
	return vec * scalar;
}

typedef Vector<int,2> vec2i;
typedef Vector<int,3> vec3i;
typedef Vector<int,4> vec4i;

typedef Vector<float,2> vec2f;
typedef Vector<float,3> vec3f;
typedef Vector<float,4> vec4f;

typedef Vector<double,2> vec2d;
typedef Vector<double,3> vec3d;
typedef Vector<double,4> vec4d;

typedef Vector<unsigned int,2> vec2u;
typedef Vector<unsigned int,3> vec3u;
typedef Vector<unsigned int,4> vec4u;

}


