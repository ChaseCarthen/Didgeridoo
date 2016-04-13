/*
   Filename : Texture.cpp
   Author   : Cody White and Joe Mahsman
   Version  : 1.0

   Purpose  : Encapsulate OpenGL texture calls. 

   Change List:

      - 06/18/2009  - Created (Cody White and Joe Mahsman)

	  - 09/16/2009  - Changed to use FreeImage instead of DevIL.
*/

#include <Texture.h>

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <util.h>

using namespace std;

#define VERBOSE

namespace gfx
{

Texture::Texture()
{
	_width = 0;
	_height = 0;
	_data = NULL;
	texture_name = "";
}

Texture::~Texture()
{
	if (valid())
	{
		gfx::ContextBuffer<GLuint>::ContextBufferIterator iter;
		for (iter = _ids.begin ();
			 iter != _ids.end ();
			 ++iter)
		{
			glDeleteTextures(1, &(iter->second));
		}
	}
}

void Texture::destroyContext (int context_id)
{
	glDeleteTextures (1, &_ids[context_id]);
	_ids.remove (context_id);
}

void Texture::load(const char* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	FIBITMAP* unconvertedData = FreeImage_Load(format, filename);
	if (unconvertedData == NULL)
	{
		stringstream error;
		error << "Texture::load(): Unable to load \"" << filename << "\".\n";
		throw runtime_error(error.str ());
	}

	_data = FreeImage_ConvertTo32Bits(unconvertedData);
	FreeImage_Unload(unconvertedData);
	_width = FreeImage_GetWidth(_data);
	_height = FreeImage_GetHeight(_data);
}

void Texture::createTextureObject(int context_id)
{
	GLuint& id = _ids[context_id];

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	applyParams();  

	// Get the maximum texture size for this GPU.
	GLint max_size = 0;
	glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max_size);
	if (_width > max_size || _height > max_size)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,
	                  _params.internalFormat,
	                  _width, _height,
	                  GL_BGRA,
	                  GL_UNSIGNED_BYTE,
	                  FreeImage_GetBits (_data));
		checkGLErrors("Texture::createTextureObject - gluBuild2DMipmaps");
	}
	else
	{
		glTexImage2D (GL_TEXTURE_2D, 0, _params.internalFormat, _width, _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits (_data));
		glGenerateMipmapEXT (GL_TEXTURE_2D);
		checkGLErrors("Texture::createTextureObject - glTexImage2D");
	}

	unbind();
}

// Determine if this texture has loaded any data or not.
bool Texture::hasData (void)
{
	return (_data != NULL);
}

void Texture::clearCPUData()
{
	if (_data)
	{
		FreeImage_Unload (_data);
		_data = NULL; 
	}
}

void Texture::blank (const size_t width, const size_t height, int data_type, int context_id)
{
	GLuint &id = _ids[context_id];
	glGenTextures (1, &id);
	glBindTexture (GL_TEXTURE_2D, id);

	applyParams ();
	glTexImage2D (GL_TEXTURE_2D, 0, _params.internalFormat, width, height, 0, _params.format, data_type, NULL);
	glBindTexture (GL_TEXTURE_2D, 0);
}

void Texture::createMipmaps (int context_id)
{
	if (valid (context_id))
	{
		bind (context_id);
		glGenerateMipmapEXT (GL_TEXTURE_2D);
		unbind ();
	}
}

void Texture::bind (int context_id) 
{
	GLuint id = _ids[context_id];
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind (void) const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Texture::applyParams()
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, _params.envMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _params.magFilter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _params.wrapT);
}

}

