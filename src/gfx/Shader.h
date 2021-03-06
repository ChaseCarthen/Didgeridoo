/*
   Filename : Shader.h
   Author   : Cody White and Joe Mahsman
   Version  : 1.0

   Purpose  : Encapsulates OpenGL shader functionality. 

   Change List:

      - 06/18/2009  - Created (Cody White and Joe Mahsman)
*/


#pragma once

#include <GL/glew.h>
#include <string>
#include <ContextBuffer.h>

namespace gfx
{

/**
 * Encapsulates the three GLSL shader types.
 */
class Shader
{
	public:

		enum Type {	VERTEX, GEOMETRY, FRAGMENT };

		Shader() {}
		~Shader();

		/**
		  * Delete a context.
		  * @param context_id Context to delete.
		  */
		void deleteContext (int context_id);

		/**
		 * Load the shader code from a text file and compile it
		 * as the specified shader type.
		 */
		void load(const char* filename);

		/**
		  * Initialize the shader for use.
		  */
		void initialize (Type type, int context_id = 0);

		/**
		  * Delete a context.
		  * @param context_id ID of context to destroy.
		  */
		void destroyContext (int context_id);
		
		/**
		 * Returns the shader object ID assigned by OpenGL.
		 */
		GLuint id (int context_id = 0) { return _ids[context_id]; }

	private:

		gfx::ContextBuffer <GLuint> _ids;
		std::string _source;

		std::string readTextFile(const char *filename);
		void compile (int context_id);
		void createShader (Type type, int context_id = 0);
};

}

