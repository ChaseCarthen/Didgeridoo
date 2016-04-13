/*
   Filename : Texture.h
   Author   : Cody White and Joe Mahsman
   Version  : 1.0

   Purpose  : Encapsulate OpenGL texture calls. 

   Change List:

      - 06/18/2009  - Created (Cody White and Joe Mahsman)
*/

#pragma once

#include <GL/glew.h>
#include <ContextBuffer.h>
#include <FreeImage.h>
#include <string>

namespace gfx
{

/**
 * Encapsulates an simple 2D texture.
 */
class Texture
{
	public:

		/**
		  * Default constructor.
		  */
		Texture();

		/**
		  * Default destructor.
		  */
		~Texture();

		/**
		  * Destory a context.
		  * @param context_id ID of the context to destroy.
		  */
		void destroyContext (int context_id);

		struct Params;

		/**
		 * Sets the parameters used when creating the texture during load().
		 * See Texture::Params for a list of members and default values.
		 * @param p Parameters to set.
		 */
		void setParams(const Params& p) { _params = p; }

		/**
		 * Load an image file using FreeImage and load it as an OpenGL texture.
		 * @param filename Path to the texture to load.
		 */
		void load(const char* filename);

		/**
		 * Creates the context-specific texture object and uploads the image data.
		 * @param context_id ID of the context to create the texture for.
		 */
		void createTextureObject(int context_id = 0);

		/**
		 * Clears image data from core memory.
		 */
		void clearCPUData();
		
		/**
		  * Determine if this texture has any data loaded or not.
		  */
		bool hasData (void);

		/**
		  * Load a blank texture.
		  * @param width Width of the blank texture to create.
		  * @param height Height of the blank texture to create.
		  * @param data_type Type of texture to create (e.g. GL_RGBA).
		  */
		void blank (const size_t width, const size_t height, int data_type, int context_id = 0);

		void bind (int context_id = 0);
		void unbind (void) const;

		/**
		 * Returns whether the texture object has been generated.
		 */
		bool valid (int context_id = 0) { return _ids[context_id] != 0; }

		/**
		  * Create mipmaps for this texture.
		  * WARNING: Only call this function if you used Texture::blank () to
		  * create the texture and wish for mipmaps to be created, otherwise
		  * they have alreay been created.
		  */
		void createMipmaps (int context_id = 0);
		
		/**
		 * Returns the texture object ID assigned by OpenGL via glGenTextures().
		 */
		const GLuint id (int context_id = 0) { return _ids[context_id]; }
		
		GLsizei width() const { return _width; }
		GLsizei height() const { return _height; }

		/**
		 * Contains parameters for texture operation.
		 * Pass this into a constructor or setParams().
		 */
		struct Params
		{
			/**
			 * number of color components
			 */
			GLint internalFormat;

			/**
			 * format of a color component
			 */
			GLenum format;

			/**
			 * Determines how s texture coordinates outside of [0..1]
			 * are handled.
			 */
			GLenum wrapS;

			GLenum wrapT;

			/**
			 * Defines the texturing function, used by OpenGL when a texture
			 * is applied to a primitive.
			 */
			GLint envMode;

			/**
			 * Minification filter used when a pixel contains
			 * multiple texels (e.g. the textured object is far).
			 */
			GLenum minFilter;

			/**
			 * Magnification filter used when a pixel is contained
			 * within a texel (e.g. the textured object is close).
			 * May only be GL_NEAREST or GL_LINEAR.
			 */
			GLenum magFilter;

			Params()
			{
				internalFormat = 3;
				format = GL_RGB;
				envMode = GL_MODULATE;
				wrapS = GL_REPEAT;
				wrapT = GL_REPEAT;	
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
				magFilter = GL_LINEAR;
			}
		};

		std::string texture_name;

	private:

		gfx::ContextBuffer <GLuint> _ids;
		GLsizei _width, _height;

		Params _params;

		FIBITMAP* _data;

		/**
		 * Set texture parameters through OpenGL API calls.
		 */
		void applyParams();
};
}

