#pragma once

#include <cavr/cavr.h>

using namespace cavr::math;

namespace cavr
{

/**
 * Represents affine transformations and their inverse transformations.
 */
class Transform
{
public:
	Transform();

	/**
	 * Rotates about an arbitrary axis intersecting the origin.
	 */
	void rotate(vec3f axis, double angle);

	/**
	 * Translates the origin.
	 */
	void translate(vec3f translation);

	/**
	 * Reverts the transformation to an identity matrix.
	 */
	void reset();

	/**
	 * Returns the transformation in matrix form.
	 */
	mat4f& matrix();

	/**
	 * Returns the inverse transformation in matrix form.
	 */
	mat4f& inverse();

	vec4f toVirtualPoint(const vec4f& point);
	vec4f toVirtualDirection(const vec4f& direction);
	vec4f toRealPoint(const vec4f& point);
	vec4f toRealDirection(const vec4f& direction);
private:
	mat4f _matrix; ///Matrix representation of the transform.
	mat4f _inverse; ///Matrix representation of the inverse.
};

}
