#include <Transform.h>

namespace cavr
{

Transform::Transform()
{
	reset();
}

void Transform::rotate(vec3f axis, double angle)
{
	mat4f m;
	m[3][3] = 1;
	double c = cos(angle);
	double s = sin(angle);
	double t = 1.0 - c;
	double x = axis.x;
	double y = axis.y;
	double z = axis.z;
	double txx = t * x * x;
	double txy = t * x * y;
	double txz = t * x * z;
	double tyy = t * y * y;
	double tyz = t * y * z;
	double tzz = t * z * z;
	double xs = x * s;
	double ys = y * s;
	double zs = z * s;
	m[0][0] = txx + c;	m[0][1] = txy - zs;	m[0][2] = txz + ys;
	m[1][0] = txy + zs;	m[1][1] = tyy + c;	m[1][2] = tyz - xs;
	m[2][0] = txz - ys;	m[2][1] = tyz + xs;	m[2][2] = tzz + c;
	_matrix = m * _matrix;
	c = cos(-angle);
	s = sin(-angle);
	t = 1.0 - c;
	x = axis.x;
	y = axis.y;
	z = axis.z;
	txx = t * x * x;
	txy = t * x * y;
	txz = t * x * z;
	tyy = t * y * y;
	tyz = t * y * z;
	tzz = t * z * z;
	xs = x * s;
	ys = y * s;
	zs = z * s;
	m[0][0] = txx + c;	m[0][1] = txy - zs;	m[0][2] = txz + ys;
	m[1][0] = txy + zs;	m[1][1] = tyy + c;	m[1][2] = tyz - xs;
	m[2][0] = txz - ys;	m[2][1] = tyz + xs;	m[2][2] = tzz + c;
	_inverse = _inverse * m;
}

void Transform::translate(vec3f translation)
{
	mat4f m;
	for (int i = 0; i < 4; ++i)
	{
		m[i][i] = 1;
	}
	m[0][3] = -translation.x;
	m[1][3] = -translation.y;
	m[2][3] = -translation.z;
	_matrix = m * _matrix;
	m[0][3] = translation.x;
	m[1][3]= translation.y;
	m[2][3] = translation.z;
	_inverse = _inverse * m;
}

void Transform::reset()
{
	for (int i = 0; i < 16; ++i)
	{
		_matrix.v[i] = 0;
		_inverse.v[i] = 0;
	}
	for (int i = 0; i < 4; ++i)
	{
		_matrix[i][i] = 1;
		_inverse[i][i] = 1;
	}
}

mat4f& Transform::matrix() 
{
	return _matrix;
}

mat4f& Transform::inverse()
{
	return _inverse;
}

vec4f Transform::toVirtualPoint(const vec4f& point)
{
	vec4f result = point;
	result.w = 1.0;
	return _inverse * result;	
}

vec4f Transform::toVirtualDirection(const vec4f& direction)
{
	vec4f result = direction;
	result.w = 0.0;
	return _inverse * result;	
}

vec4f Transform::toRealPoint(const vec4f& point)
{
	vec4f result = point;
	result.w = 1.0;
	return _matrix * result;	
}

vec4f Transform::toRealDirection(const vec4f& direction)
{
	vec4f result = direction;
	result.w = 0.0;
	return _matrix * result;	
}

}
