#include <Uniform.h>

namespace gfx
{

Uniform::Uniform()
{
	_pid = 0;
	_location = -1;
}

Uniform::Uniform(GLuint pid, const char* name)
{
	_pid = pid;
	_location = glGetUniformLocation(_pid, name);

	if (_location == -1)
	{
		throw("Could not find uniform.");
	}
}

void Uniform::set1i(int i) const
{
	glUniform1i(_location, i);
}

void Uniform::set1f(float f) const
{
	glUniform1f(_location, f);
}

int Uniform::get1i() const
{
	int i;
	glGetUniformiv(_pid, _location, &i);
	return i;
}

float Uniform::get1f() const
{
	int f;
	glGetUniformiv(_pid, _location, &f);
	return f;
}

}
