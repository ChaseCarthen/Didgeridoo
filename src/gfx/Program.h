/*
   Filename : Program.h
   Author   : Cody White and Joe Mahsman
   Version  : 1.0

   Purpose  : Contains several shaders to makeup a shader program. 

   Change List:

      - 06/18/2009  - Created (Cody White and Joe Mahsman)
*/

#pragma once

#include <GL/glew.h>
#include <Shader.h>
#include <Uniform.h>
#include <ContextBuffer.h>
#include <cstring>
#include <string>
#include <map>

namespace gfx
{

/**
 * Encapsulates a GLSL program.
 */
class Program
{
	public:

		/**
		  * Default constructor.
		  */
		Program();

		/**
		  * Destructor.
		  */
		~Program();

		/**
		 * Attach the specified shader.
		 * The shader must already be loaded using Shader::load().
		 * @param shader Shader to attach to this program.
		 */
		void attach(Shader& shader, int context_id = 0);

		/**
		 * Link the program.
		 * Call this after all shaders have been attached.
		 */
		void link (int context_id = 0);

		/**
		  * Destroy the context for this program.
		  */
		void destroyContext (int context_id);
 
		/**
		  * Set an integer uniform in the program.
		  * @param name Name of the uniform.
		  * @param i Value of the uniform.
		  */
		void set1i(const char* name, int i) const;

		/**
		  * Set a float uniform in the program.
		  * @param name Name of the uniform.
		  * @param f Value of the uniform.
		  */
		void set1f(const char* name, float f) const;

		/**
		  * Set 3 component double uniform in the program.
		  * @param name Name of the uniform.
		  * @param d Value of the uniform.
		  */
		void set3dv(const char* name, double *d) const;

		/**
		  * Set an 3 component float uniform in the program.
		  * @param name Name of the uniform.
		  * @param f Value of the uniform.
		  */
		void set3fv(const char* name, float *f) const;

		/**
		  * Set an 4 component uniform in the program.
		  * @param name Name of the uniform.
		  * @param d Value of the uniform.
		  */
		void set4iv(const char* name, int *d) const;

		/**
		  * Bind this program for use.
		  */
		void bind (int context_id = 0) const;

		/**
		  * Unbind this program from use.
		  */
		void unbind() const;

		/**
		  * Get the id of this program.
		  */
		GLuint id (int context_id = 0) const;

		/**
		  * Get a location of an attribute variable in the program.
		  * @param name Name of the attribute variable.
		  */
		GLint getAttributeLocation (const char *name, int context_id = 0)
		{
			return glGetAttribLocationARB (id (context_id), name);
		}

	private:

		gfx::ContextBuffer <GLuint> _ids;

		struct ltstr
		{
			bool operator()(const char* s1, const char* s2) const
			{
				return strcmp(s1, s2) < 0;
			}
		};

		//mutable std::map<const char*, GLint, ltstr> _uniformCache;
		mutable std::map<std::string, GLint> _uniformCache;

		typedef std::map <std::string, GLint>::iterator CacheIterator;

		/**
		 * Iterate through all active uniforms and store their locations.
		 * Must be called after linking.
		 */
		void cacheUniforms (int context_id);
		void create (int context_id);
};
}

