/*
   Filename : Program.cpp
   Author   : Cody White and Joe Mahsman
   Version  : 1.0

   Purpose  : Contains several shaders to makeup a shader program. 

   Change List:

      - 06/18/2009  - Created (Cody White and Joe Mahsman)
*/

#include <Program.h>
#include <util.h>
#include <cstdlib>
#include <iostream>
using namespace std;

namespace gfx
{

Program::Program()
{
}

Program::~Program()
{
	gfx::ContextBuffer<GLuint>::ContextBufferIterator iter;
	for (iter = _ids.begin (); iter != _ids.end (); ++iter)
	{
		glDeleteProgram (iter->second);
	}
}

void Program::attach (Shader& shader, int context_id)
{
	create (context_id);
	glAttachShader (_ids[context_id], shader.id (context_id));
	checkGLErrors("Program::attach () - Attach shader to program");
}

void Program::link (int context_id)
{
	GLuint &id = _ids[context_id];
	GLint success;
	const unsigned int bufferSize = 5000;
	char* buffer = new char[bufferSize];

	glLinkProgram(id);
	checkGLErrors("Program::link () - Link the program");

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	checkGLErrors("Program::link () - Determine successful link");

	if (!success)
	{
		glGetProgramInfoLog(id, bufferSize, 0, buffer);
		printf("%s\n", buffer);

		delete [] buffer;
		exit(1);
	}

	cacheUniforms (context_id);
}

void Program::destroyContext (int context_id)
{
	glDeleteProgram (_ids[context_id]);
	_ids.remove (context_id);
}

void Program::set1i(const char* name, int i) const
{
	CacheIterator iter = _uniformCache.find (name);
	if (iter != _uniformCache.end())
	{
		glUniform1i (iter->second, i);
	}
	else
	{
		//cout << "set1i error: " << name << endl;
	}
}

void Program::set1f(const char* name, float f) const
{
	CacheIterator iter = _uniformCache.find (name);
	if (iter != _uniformCache.end())
	{
		glUniform1f (iter->second, f);
	}
	else
	{
		//cout << "set1f error: " << name << endl;
	}
}

void Program::set3dv(const char* name, double *d) const
{
	CacheIterator iter = _uniformCache.find (name);
	if (iter != _uniformCache.end())
	{
		float f[3];
		f[0] = (float)d[0];
		f[1] = (float)d[1];
		f[2] = (float)d[2];
		
		glUniform3fv (iter->second, 1, f);
	}
	else
	{
		//cout << "set3dv error: " << name << endl;
	}
}

void Program::set3fv(const char* name, float *f) const
{
	CacheIterator iter = _uniformCache.find (name);
	if (iter != _uniformCache.end())
	{
		glUniform3fv(iter->second, 1, f);
	}
	else
	{
		//cout << "set3fv error: " << name << endl;
	}
}

void Program::set4iv(const char* name, int *d) const
{
	CacheIterator iter = _uniformCache.find (name);
	if (iter != _uniformCache.end())
	{
		glUniform4iv(iter->second, 1, d);
	}
	else
	{
		//cout << "set4iv error: " << name << endl;
	}
}

void Program::bind (int context_id) const
{
	glUseProgram(_ids[context_id]);
}

void Program::unbind() const
{
	glUseProgram(0);
}

// Get the id of this program.
GLuint Program::id (int context_id) const
{
	return _ids[context_id];
}

void Program::cacheUniforms (int context_id)
{
	GLuint &id = _ids[context_id];
	// Get number of active uniform variables
	GLint activeUniforms;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &activeUniforms);

	const int bufferSize = 200;
	char* nameBuffer = new char[bufferSize];

	GLsizei numChars;
	GLint size;
	GLenum dataType;

	// Get information on each
	for (int i = 0; i < activeUniforms; ++i)
	{
		glGetActiveUniform(id,
		                   i,
		                   bufferSize,
		                   &numChars,
		                   &size,
		                   &dataType,
		                   nameBuffer);

		char array[4];
		array[0] = nameBuffer[0];
		array[1] = nameBuffer[1];
		array[2] = nameBuffer[2];
		array[3] = 0;
		if (strcmp("gl_", array) == 0)
		{
			continue;
		}
		GLint location = glGetUniformLocation(id, nameBuffer);

		if (location == -1)
		{
			std::cout << "Program::cacheUniforms () - Error: Unable to cache uniform because a valid unifom location could not be found!" << std::endl;
			throw 0;
		}

		//TODO: This could be an issue -- it may need to be context safe
		_uniformCache[nameBuffer] = location;
	}

}

void Program::create (int context_id)
{
	GLuint &id = _ids[context_id];
	if (glIsProgram (id))
	{
		return;
	}
	id = glCreateProgram();

	if (id == 0)
	{
		std::cout << "Program::create () - Error: OpenGL could not create the program!" << std::endl;
		throw 0;
	}
}
}


