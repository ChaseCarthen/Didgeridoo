/*
   Filename : Triangle.h
   Author   : Cody White
   Version  : 1.0

   Purpose  : Class to define a triangle. 

   Change List:

      - 06/18/2009  - Created (Cody White)
*/

#pragma once

#include <Vector.h>
#include <Material.h>
#include <vector>

namespace gfx
{

/**
  * Class to contain one triangle.
  */
template <typename T>
class Triangle
{
	public:

		/**
		  * Default constructor.
		  */
		Triangle (void)
		{
			m_vertices[0].zero ();
			m_vertices[1].zero ();
			m_vertices[2].zero ();
			
			m_texture_coords[0].zero ();
			m_texture_coords[1].zero ();
			m_texture_coords[2].zero ();

			m_normals[0].zero ();
			m_normals[1].zero ();
			m_normals[2].zero ();
			
			m_tangents[0].zero ();
			m_tangents[1].zero ();
			m_tangents[2].zero ();
			
			m_has_split = false;
			m_material = NULL;
		}

		/**
		  * Destructor.
		  */
		~Triangle (void)
		{
		}

		/**
		  * Copy Constructor.
		  * @param rhs Triangle to copy.
		  */
		Triangle (const Triangle &rhs)
		{
			*this = rhs;
		}

		/**
		  * Operator =.
		  * @param rhs Triangle to set this one equal to.
		  */
		Triangle & operator= (const Triangle &rhs)
		{
			if (this != &rhs)
			{
				m_vertices[0] = rhs.m_vertices[0];
				m_vertices[1] = rhs.m_vertices[1];
				m_vertices[2] = rhs.m_vertices[2];
				m_texture_coords[0] = rhs.m_texture_coords[0];
				m_texture_coords[1] = rhs.m_texture_coords[1];
				m_texture_coords[2] = rhs.m_texture_coords[2];
				m_tangents[0] = rhs.m_tangents[0];
				m_tangents[1] = rhs.m_tangents[1];
				m_tangents[2] = rhs.m_tangents[2];
				m_normals[0]  = rhs.m_normals[0];
				m_normals[1]  = rhs.m_normals[1];
				m_normals[2]  = rhs.m_normals[2];
				m_material = rhs.m_material;
				m_has_split = rhs.m_has_split;
				m_b = rhs.m_b;
				m_c = rhs.m_c;
				m_N = rhs.m_N;
				m_barycentric_divide = rhs.m_barycentric_divide;
				m_u = rhs.m_u;
				m_v = rhs.m_v;
			}

			return *this;
		}

		/**
		  * Operator[]
		  * @param i Index of vertex to return.
		  */
		inline math::Vector <T, 3> & operator[] (int i)
		{
			return m_vertices[i];
		}

		/**
		  * Calculate an interpolated normal for a hit point on this triangle.
		  * @param hit_point The hit point on this triangle.
		  */
		math::Vector <T, 3> interpolate (math::Vector <T, 3> hit_point)
		{
			math::Vector <T, 3> normal = math::normalize (math::cross (m_vertices[1] - m_vertices[0], m_vertices[2] - m_vertices[0]));
			T whole_area = math::dot (normal, math::cross (m_vertices[1] - m_vertices[0], m_vertices[2] - m_vertices[0]));

			// Compute first barycentric coordinate
			T area_p12 = math::dot (normal, math::cross (m_vertices[1] - hit_point, m_vertices[2] - hit_point));
			T a = area_p12 / whole_area;

			// Compute the second barycentric coordinate
			T area_p20 = math::dot (normal, math::cross (m_vertices[2] - hit_point, m_vertices[0] - hit_point));
			T b = area_p20 / whole_area;

			// Compute the last barycentric coordinate (a + b + c = 0)
			T c = (T)1 - a - b;

			return math::normalize (m_normals[0] * a + m_normals[1] * b + m_normals[2] * c);
		}

		/**
		  * Check for a ray collision.
		  * @param ray_origin Origin of the incoming ray.
		  * @param ray_direction Direction of the incoming ray.
		  * @param time Time of intersection.
		  */
		bool checkRayIntersection (math::Vector <T, 3> &ray_origin, math::Vector <T, 3> &ray_direction, T &time)
		{
			time = -math::dot (ray_origin - m_vertices[0], m_N) / math::dot (ray_direction, m_N);
			if (time < 0.0000001)
			{
				return false;
			}

			math::Vector <T, 3> H;
			H[m_u] = ray_origin[m_u] + time * ray_direction[m_u] - m_vertices[0][m_u];
			H[m_v] = ray_origin[m_v] + time * ray_direction[m_v] - m_vertices[0][m_v];

			T beta = (m_b[m_u] * H[m_v] - m_b[m_v] * H[m_u]) * m_barycentric_divide;
			if (beta < 0)
			{
				return false;
			}

			T gamma = (m_c[m_v] * H[m_u] - m_c[m_u] * H[m_v]) * m_barycentric_divide;
			if (gamma < 0)
			{
				return false;
			}

			if (beta + gamma > 1)
			{
				return false;
			}

			return true;
		}

		/**
		  * Calculate the tangent at each of the vertices.  This is used for eye-space conversion.
		  */
		void calcTangents (void)
		{
			m_tangents[0] = getTangent (m_vertices[0], m_vertices[1], m_vertices[2], m_texture_coords[0], m_texture_coords[1], m_texture_coords[2]);	
			m_tangents[1] = getTangent (m_vertices[1], m_vertices[2], m_vertices[0], m_texture_coords[1], m_texture_coords[2], m_texture_coords[0]);	
			m_tangents[2] = getTangent (m_vertices[2], m_vertices[0], m_vertices[1], m_texture_coords[2], m_texture_coords[0], m_texture_coords[1]);	
		}

		/**
		  * Initialize intersection.
		  */
		void initializeIntersection (void)
		{
			m_b = m_vertices[2] - m_vertices[0];
			m_c = m_vertices[1] - m_vertices[0];
			m_N = math::cross (m_c, m_b);

			int k;
			if (fabs (m_N.x ()) > fabs (m_N.y ()))
			{
				if (fabs (m_N.x ()) > fabs (m_N.z ()))
				{
					k = 0;
				}
				else
				{
					k = 2;
				}
			}
			else
			{
				if (fabs (m_N.y ()) > fabs (m_N.z ()))
				{
					k = 1;
				}
				else
				{
					k = 2;
				}
			}

			int mod[] = {0, 1, 2, 0, 1};
			m_u = mod[k + 1];
			m_v = mod[k + 2];
			m_barycentric_divide = 1.0 / (m_b[m_u] * m_c[m_v] - m_b[m_v] * m_c[m_u]);
		}

		// Member variables
		math::Vector <T, 3> m_vertices [3];			// Vertices of this triangle.
		math::Vector <T, 2> m_texture_coords [3];	// Texture coordinates that correspond to the vertices of the triangle.
		math::Vector <T, 4> m_tangents [3];			// Tangent vectors at each vertex.
		math::Vector <T, 3> m_normals [3];			// Normals of this triangle at each vertex.
		gfx::Material	    *m_material;			// Pointer to a material.
		
		bool  m_has_split;	// Flag for determining if this triangle has tesselated data.	

		std::vector <Triangle<T> *> m_tesselations;	// The tesselated version of this triangle.

		// Intersection variables.
		math::Vector <T, 3> m_b, m_c, m_N;
		T m_barycentric_divide;
		T m_u, m_v;

	private:
	
		/**
		  * Compute the tangent based on the passed in vertices and texture coordinates.
		  * @param v1 Vertex 1 of the triangle.
		  * @param v2 Vertex 2 of the triangle.
		  * @param v3 Vertex 3 of the triangle.
		  * @param t1 Texture coordinate 1 that corresponds to v1.
		  * @param t2 Texture coordinate 2 that corresponds to v2.
		  * @param t3 Texture coordinate 3 that corresponds to v3.
		  */
		math::Vector <T, 4> getTangent (math::Vector <T, 3> &v1, math::Vector <T, 3> &v2, math::Vector <T, 3> &v3, math::Vector <T, 2> &t1, math::Vector <T, 2> &t2, math::Vector <T, 2> &t3)
		{
			T det = (T)1 / (((t2[0] - t1[0]) * (t3[1] - t1[1])) - ((t3[0] - t1[0]) * (t2[1] - t1[1])));
			math::Vector <T, 3> t, b;

			math::Vector <T, 3> edge1 = v2 - v1;
			math::Vector <T, 3> edge2 = v3 - v1;

			t = ((edge1 * (t3[1] - t1[1])) - (edge2 * (t2[1] - t1[1]))) * det;
			b = ((edge1 * (-(t3[0] - t1[0]))) + (edge2 * (t2[0] - t1[0]))) * det;
			t = normalize (t);
			b = normalize (b);
			math::Vector <T, 3> normal = cross (t, b);

			// Calculate the handedness of the local tangent space (right or left) which is
			// dependent upon the bitangent.  The bitangent vector is the cross product between
			// the triangle face normal and the tangent vector.  The resulting bitangent vector
			// should be the same as the bitangend vector calculated from the set of equations above.
			// If the point in different directions, then the bitangent that will be calculated in the
			// shader needs to be inverted to perform correct lighting.
			math::Vector <T, 3> bitangent = cross (normal, t);
			T handedness = dot (bitangent, b) < (T)0 ? (T)1 : -(T)1;

			math::vec4f tangent;
			tangent[0] = t[0];
			tangent[1] = t[1];
			tangent[2] = t[2];
			tangent[3] = handedness;
			return tangent;
		}
};

}


