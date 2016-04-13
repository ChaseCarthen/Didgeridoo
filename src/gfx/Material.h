/*
   Filename : Material.h
   Author   : Cody White
   Version  : 1.0

   Purpose  : Definition of a material for a .obj file. 

   Change List:

      - 12/21/2009  - Created (Cody White)
*/

#pragma once

#include <Vector.h>
#include <Texture.h>
#include <string>

namespace gfx
{

struct Material
{
	Material (void)
	{
		name = "";
		texture.texture_name = "";
		diffuse.zero ();
		specular.zero ();
		transmissive.zero ();
		specular_exponent = 0.0f;
		alpha = 0.0f;
		index_of_refraction = 0.0f;
	}

	std::string name;			// Name of the material.
	math::vec3f diffuse;		// Diffuse component.
	math::vec3f specular;		// Specular component.
	math::vec3f transmissive;	// Transmissive component.
	float specular_exponent;	// Specular exponent.
	float alpha;				// Alpha value for this material.
	float index_of_refraction;	// Index of refraction for this material.
	gfx::Texture texture;		// Texture which is bound to this material.
};

}

