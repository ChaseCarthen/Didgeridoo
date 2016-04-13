/*
   Filename : Shader.cpp
   Author   : Cody White and Joe Mahsman
   Version  : 1.0

   Purpose  : Encapsulates OpenGL shader functionality. 

   Change List:

      - 06/18/2009  - Created (Cody White and Joe Mahsman)
*/

#include <Shader.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <util.h>
#include <cstring>
#include <fstream>

#define VERBOSE

namespace gfx
{

Shader::~Shader()
{
	gfx::ContextBuffer<GLuint>::ContextBufferIterator iter;
	for (iter = _ids.begin (); iter != _ids.end (); ++iter)
	{
		glDeleteShader(iter->second);
	}
}

void Shader::destroyContext (int context_id)
{
	glDeleteShader (_ids[context_id]);
	_ids.remove (context_id);
}

std::string Shader::readTextFile(const char *filename)
{
	std::string content = "";
	std::ifstream fin;
	fin.open (filename);
	if (!fin)
	{
		return content;
	}

	std::string tmp;
	while (fin.good ())
	{
		tmp = fin.get ();
		if (fin.good ())
		{
			content += tmp;
		}
	}

	fin.close ();

	return content;
}

void Shader::load(const char* filename)
{
	_source = readTextFile(filename);

	if (_source == "")
	{
		std::cout << "Shader::load () - Error: could not read shader source file " << filename << std::endl;
		exit(1);
	}
}

// Initialize a shader for use.
void Shader::initialize (Type type, int context_id)
{
	const GLchar *s = _source.c_str ();
	createShader (type, context_id);
	glShaderSource(_ids[context_id], 1, &s, NULL);
	checkGLErrors("Shader::initialize () - set shader source");
	compile (context_id);
	checkGLErrors("Shader::initialize () - compilation complete");

}

void Shader::compile (int context_id)
{
	GLuint id = _ids[context_id];
	GLint success;
	const unsigned int bufferSize = 5000;
	char* buffer = new char[bufferSize];
	memset (buffer, 0, bufferSize);

	glCompileShader(id);
	checkGLErrors("Shader::compile () - compiling shader");

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	glGetShaderInfoLog(id, bufferSize, 0, buffer);
	if (buffer[0] != 0)
	{
		printf("%s\n", buffer);
	}
	
	delete [] buffer;
}

void Shader::createShader(Type type, int context_id)
{
	GLenum glType;

	switch (type)
	{
	case VERTEX:
		glType = GL_VERTEX_SHADER;
		break;

	case GEOMETRY:
		glType = GL_GEOMETRY_SHADER_EXT;
		break;

	case FRAGMENT:
		glType = GL_FRAGMENT_SHADER;
		break;

	default:
		std::cout << "Shader::createShader () - Error: Unknown type \"" << type << "\" specified as shader type. " 
			      << "Acceptable types are \"VERTEX\", \"GEOMETRY\", or \"FRAGMENT\"" << std::endl;
		exit(1);
	}

	GLuint &id = _ids[context_id];
	checkGLErrors("Shader::createShader () - shader_type_set");
	id = glCreateShader(glType);
	checkGLErrors("Shader::createShader () - shader_id_created");

	if (id == 0)
	{
		std::cout << "Shader::createShader () - Error: Texture id == 0, shader could not be created!" << std::endl;
		exit(1);
	}
}

}

