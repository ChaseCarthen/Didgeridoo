/*
   Filename : Mesh.h
   Author   : Cody White
   Version  : 1.1

   Purpose  : Class to define a triangle mesh. 

   Change List:

      - 12/21/2009  - Created (Cody White)
	  - 01/27/2010  - Templated the class (Cody White)
	  - 02/26/2010  - Added texturing to the mesh.
*/

#pragma once

#include <Triangle.h>
#include <Vector.h>
#include <VertexBuffer.h>
#include <Material.h>
#include <ContextBuffer.h>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

namespace gfx
{

/**
  * Class to define a triangle mesh.
  */
template <class T>
class Mesh
{
	public:

		typedef typename std::map <Material *, std::vector <gfx::Triangle <T> > >::iterator TriangleIterator; 

		/**
		  * Default constructor.
		  */
		Mesh (void)
		{
			m_sizeof_render_data = sizeof(RenderData);
			m_use_normals 	= false;
			m_use_texture 	= false;
			m_use_materials = false;
			m_use_tangents 	= false;
		}

		/** 
		  * Default destructor.
		  */
		~Mesh (void)
		{
			// Clear all internal lists.
			clearCPUData ();
		}

		/**
		  * Indicates that tangents should be calculated and rendered.
		  * Default is false.
		  */
		void useTangents (bool useTangents) { m_use_tangents = useTangents; }

		/**
		  * Sets the attribute location of the tangent attribute in the GLSL program.
		  */
		void setTangentAttributeLocation (GLint tangent_loc, int id) { m_tangent_loc[id] = tangent_loc; }

		/**
		  * Load a mesh from a filename, must be .obj.
		  * @param filename Path to the .obj file.
		  * @param create_vbo Flag to create a vbo after loading the file.
		  */
		bool load (const char *filename, bool create_vbo = false)
		{
			std::ifstream fin;
			std::string path = filename;
			path = path.substr (0, path.find_last_of ("/") + 1);

			// Open the input file.
			fin.open (filename);
			if (!fin)
			{
				std::cout << "Mesh::load () - Error: Could not open .obj file: " << filename << std::endl;
				return false;
			}

			std::string input_line       = "";
			std::string current_material = "default";

			int num_tris = 0;

			// Add a default_material for use if no material file is specified in the obj file.
			gfx::Material default_material;
			default_material.name = "default";
			default_material.specular_exponent = 1.0;
			default_material.transmissive = math::Vector <T, 3> (0, 0, 0);
			default_material.diffuse = math::Vector <T, 3> (1, 1, 1);
			default_material.specular = math::Vector <T, 3> (0, 0, 0);
			default_material.alpha = 0;
			default_material.index_of_refraction = 1;
			m_materials[default_material.name] = default_material;

			// Read until the end of the file.
			while (fin >> input_line)
			{
				if (input_line[0] == '#')
				{
					char tmp [200];
					fin.getline (tmp, 200, '\n');
				}

				else if (input_line == "mtllib")
				{
					// Read in the material file.
					std::string material_filename = "";
					fin >> material_filename;
					material_filename = path + material_filename;

					std::ifstream mat_fin;
					mat_fin.open (material_filename.c_str ());
					if (!mat_fin)
					{
						std::cout << "Mesh::load () - Error: Could not open material file \"" << material_filename << "\" specified in \"" << filename << "\"" <<  std::endl;
						mat_fin.close ();
						fin.close ();
						return false;
					}

					std::string material_name;
					std::string material_line;
					std::map <std::string, Material>::iterator material_iter;

					while (mat_fin >> material_line)
					{
						if (material_line[0] == '#')
						{
							char tmp [200];
							mat_fin.getline (tmp, 200, '\n');
						}

						else if (material_line == "newmtl")
						{
							mat_fin >> material_name;
							m_materials[material_name].name = material_name;
							material_iter = m_materials.find (material_name);
							if (material_iter == m_materials.end ())
							{
								std::cout << "Mesh::load () - Error: Could not insert material \"" << material_name << "\" into material list." << std::endl;
								exit (0);
							}
						}

						else if (material_line == "Ns")
						{
							mat_fin >> material_iter->second.specular_exponent;
						}

						else if (material_line == "Ka")
						{
							mat_fin >> material_iter->second.transmissive[0] >> material_iter->second.transmissive [1] >> material_iter->second.transmissive[2];
						}

						else if (material_line == "Kd")
						{
							mat_fin >> material_iter->second.diffuse[0] >> material_iter->second.diffuse[1] >> material_iter->second.diffuse[2];
						}

						else if (material_line == "Ks")
						{
							mat_fin >> material_iter->second.specular[0] >> material_iter->second.specular[1] >> material_iter->second.specular[2];
						}

						else if (material_line == "Ni")
						{
							mat_fin >> material_iter->second.index_of_refraction;
						}

						else if (material_line == "d")
						{
							mat_fin >> material_iter->second.alpha;
						}

						else if (material_line == "map_Kd")
						{
							mat_fin >> material_iter->second.texture.texture_name;
						}

						else if (material_line == "illum")
						{
							int dummy;
							mat_fin >> dummy;
						}

						else
						{
							std::cout << "Mesh::load () - Error: Unknown material type: " << material_line << std::endl;
							fin.close ();
							mat_fin.close ();
							return false;
						}
					}

					mat_fin.close ();
					m_use_materials = true;
				}

				// Vertex
				else if (input_line == "v")
				{
					math::Vector <T, 3> vertex;
					fin >> vertex[0] >> vertex[1] >> vertex [2];
					m_vertices.push_back (vertex);
				}

				// Object name.
				else if (input_line == "o")
				{
					std::string name;
					fin >> name; // Ignore it.
				}

				// Vertex normal.
				else if (input_line == "vn")
				{
					math::Vector <T, 3> normal;
					fin >> normal[0] >> normal[1] >> normal[2];
					m_normals.push_back (normal);
					m_use_normals = true;
				}

				// Texture coordinate.
				else if (input_line == "vt")
				{
					math::Vector <T, 2> tex_coord;
					fin >> tex_coord[0] >> tex_coord[1];
					m_texture_coords.push_back (tex_coord);
					m_use_texture = true;
				}

				// This material is now used for all faces until told otherwise.
				else if (input_line == "usemtl")
				{
					fin >> current_material;
					m_use_materials = true;
				}

				else if (input_line == "s")
				{
					std::string smooth;
					fin >> smooth; // ignore.
				}

				// Triangle (face)
				else if (input_line == "f")
				{
					int vertex_indices 	  [3];
					int normal_indices 	  [3];
					int tex_coord_indices [3];

					std::string line;
					for (int i = 0; i < 3; ++i)
					{
						fin >> line;

						if (m_use_normals && !m_use_texture)
						{
							size_t cut = line.find ("/");
							line[cut] = ' ';
							line[cut + 1] = ' ';
							sscanf (line.c_str (), "%d %d", &vertex_indices[i], &normal_indices[i]);
						}
						else if (!m_use_normals && m_use_texture)
						{
							size_t cut = line.find ("/");
							line[cut] = ' ';
							sscanf (line.c_str (), "%d %d", &vertex_indices[i], &tex_coord_indices[i]);
						}
						else if (!m_use_normals && !m_use_texture)
						{
							sscanf (line.c_str (), "%d", &vertex_indices[i]);
						}
						else if (m_use_normals && m_use_texture)
						{
							size_t cut = line.find ("/");
							line[cut] = ' ';
							cut = line.find ("/");
							line[cut] = ' ';
							sscanf (line.c_str (), "%d %d %d", &vertex_indices[i], &tex_coord_indices[i], &normal_indices[i]);
						}
					}

					// Build the triangle from these faces.
					gfx::Triangle<T> triangle;
					triangle.m_vertices[0] = m_vertices[vertex_indices[0] - 1];
					triangle.m_vertices[1] = m_vertices[vertex_indices[1] - 1];
					triangle.m_vertices[2] = m_vertices[vertex_indices[2] - 1];

					if (m_use_normals)
					{
						triangle.m_normals[0] = m_normals[normal_indices[0] - 1];
						triangle.m_normals[1] = m_normals[normal_indices[1] - 1];
						triangle.m_normals[2] = m_normals[normal_indices[2] - 1];
					}

					if (m_use_texture)
					{
						triangle.m_texture_coords[0] = m_texture_coords[tex_coord_indices[0] - 1];
						triangle.m_texture_coords[1] = m_texture_coords[tex_coord_indices[1] - 1];
						triangle.m_texture_coords[2] = m_texture_coords[tex_coord_indices[2] - 1];
					}

					if (m_use_tangents)
					{
						triangle.calcTangents();
					}
					
					triangle.m_material = &(m_materials[current_material]);
					m_triangles[triangle.m_material].push_back (triangle);
					num_tris++;
				}
			}

			fin.close ();

			// Load any textures.
			std::map <std::string, Material>::iterator iter;
			for (iter = m_materials.begin (); iter != m_materials.end (); ++iter)
			{
				if (iter->second.texture.texture_name.length ())
				{
					iter->second.texture.load (iter->second.texture.texture_name.c_str ());
				}
			}	

			std::cout << "Loaded " << num_tris << " triangles" << std::endl;
			std::cout << "Loaded " << m_normals.size () << " normals" << std::endl;
			std::cout << "Loaded " << m_vertices.size () << " vertices" << std::endl;
			std::cout << "Loaded " << m_texture_coords.size () << " texture coordinates" << std::endl;
			std::cout << "Loaded " << m_materials.size () << " materials" << std::endl;

			if (create_vbo)
			{
				createVBO ();
			}

			m_vertices.clear ();
			m_normals.clear ();
			m_texture_coords.clear ();

			return true;
		}

		/**
		  * Initialize the textures loaded from the model file for use.
		  * This must be called before the mesh is rendered if textures are used for the model.
		  * @param context_id IF of the context to initialize the textures for.
		  */
		void initializeTextures (int context_id = 0)
		{
			std::map <std::string, Material>::iterator iter;
			for (iter = m_materials.begin (); iter != m_materials.end (); ++iter)
			{
				if (iter->second.texture.hasData ())
				{
					iter->second.texture.createTextureObject (context_id);
				}
			}
		}

		/**
		  * Create an internal VBO used for rendering.
		  * @param context_id ID of the context to create the VBO for.
		  */
		void createVBO (int context_id = 0)
		{
			// Allocate a VBO for each material found.
			m_vbos.resize (m_triangles.size ());
			int vbo_count = 0;
			for (TriangleIterator iter = m_triangles.begin (); iter != m_triangles.end (); ++iter, ++vbo_count)
			{
				if (m_use_tangents)
				{
					populateVBO2(iter, vbo_count, context_id);
				}
				else
				{
					populateVBO(iter, vbo_count, context_id);
				}
			}
		}

		/**
		  * Destroy Context.
		  * @param context_id ID of the context to destroy.
		  */
		void destroyContext (int context_id)
		{
			for (size_t i = 0; i < m_vbos.size (); ++i)
			{
				m_vbos[i].vbo.destroyContext (context_id);
			}
		}

		/**
		  * Render the mesh.
		  * @param context_id Id of the current context to render to.
		  */
		void render (int context_id = 0)  
		{
			bool bind_texture = false;
			if (m_vbos.size ())
			{
				// A VBO has been created for this mesh, so use it.
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);	
				glEnableClientState (GL_TEXTURE_COORD_ARRAY);

				if (m_use_tangents)
				{
					glEnableVertexAttribArrayARB(m_tangent_loc[context_id]);
				}

				for (size_t i = 0; i < m_vbos.size (); ++i)
				{
					if (m_use_materials)
					{
						applyMaterial ((const gfx::Material *)m_vbos[i].material);
					}

					// Apply any texture that this material might have.
					bind_texture = m_vbos[i].material->texture.valid (context_id);

					if (bind_texture)
					{
						m_vbos[i].material->texture.bind (context_id);
					}

					m_vbos[i].vbo.bind (context_id);
					glVertexPointer (3, GL_FLOAT, m_sizeof_render_data, (char *)NULL);
					glNormalPointer (GL_FLOAT, m_sizeof_render_data, (char*)NULL + sizeof (math::vec3f));
					glTexCoordPointer (2, GL_FLOAT, m_sizeof_render_data, (char *)NULL + (2 * sizeof (math::vec3f)));

					if (m_use_tangents)
					{
						glVertexAttribPointerARB (m_tangent_loc[context_id], 4, GL_FLOAT, false, m_sizeof_render_data, (char *)NULL + (2 * sizeof (math::vec3f)) + sizeof (math::vec2f));
					}

					glDrawArrays (GL_TRIANGLES, 0, m_vbos[i].num_vertices);
					m_vbos[i].vbo.unbind ();

					if (bind_texture)
					{
						m_vbos[i].material->texture.unbind ();
					}
				}

				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState (GL_TEXTURE_COORD_ARRAY);

				if (m_use_tangents)
				{
					glDisableVertexAttribArrayARB(m_tangent_loc[context_id]);
				}

				return;	
			}

			// Else render in immediate mode.
			typename std::map <Material *, std::vector <gfx::Triangle <T> > >::const_iterator iter; 
			for (iter = m_triangles.begin (); iter != m_triangles.end (); ++iter)
			{
				applyMaterial ((const gfx::Material *)iter->first);
				// Apply any texture that this material might have.
				bind_texture = iter->first->texture.valid (context_id);

				if (bind_texture)
				{
					iter->first->texture.bind (context_id);
				}

				glBegin (GL_TRIANGLES);
				for (size_t i = 0; i < iter->second.size (); ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						if (m_use_normals)
						{
							glNormal3f ((float)iter->second[i].m_normals[j].x (),
										(float)iter->second[i].m_normals[j].y (),
										(float)iter->second[i].m_normals[j].z ());
						}

						if (m_use_texture)
						{
							glTexCoord2f ((float)iter->second[i].m_texture_coords[j].x (),
										  (float)iter->second[i].m_texture_coords[j].y ());
						}

						glVertex3f ((float)iter->second[i].m_vertices[j].x (),
									(float)iter->second[i].m_vertices[j].y (),
									(float)iter->second[i].m_vertices[j].z ());
					}
				}
				glEnd ();

				if (bind_texture)
				{
					iter->first->texture.unbind ();
				}
			}
		}

		/**
		  * Clear the triangle lists.  If a vbo has been created, the internal
		  * lists can be cleared if no triangle data is needed.
		  */
		void clearCPUData (void)
		{
			m_triangles.clear ();
			
			std::map <std::string, Material>::iterator iter;
			for (iter = m_materials.begin (); iter != m_materials.end (); ++iter)
			{
				iter->second.texture.clearCPUData ();
			}
		}

		/**
		  * Get the beginning iterator to the triangle list.
		  */
		TriangleIterator begin (void)
		{
			return m_triangles.begin ();
		}

		/**
		  * Get the end iterator to the triangle list.
		  */
		TriangleIterator end (void)
		{
			return m_triangles.end ();
		}

		// Stored internal triangles from the loaded mesh.
		std::map <Material *, std::vector <gfx::Triangle <T> > > m_triangles;

	private:

		/**
		  * Copy triangles to a VBO using RenderData1.
		  * @param iter Current VBO list to populate.
		  * @param vbo_count Index of VBO being populated.
		  * @param context_id Context Id for the current graphics context.
		  */
		void populateVBO (TriangleIterator &iter, int &vbo_count, int context_id)
		{
			m_sizeof_render_data = sizeof (RenderData);

			std::vector<RenderData> data;

			data.resize (iter->second.size () * 3);
			int data_counter = 0;
			for (size_t i = 0; i < iter->second.size (); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					data[data_counter].normal = iter->second[i].m_normals[j];
					data[data_counter].vertex = iter->second[i].m_vertices[j];
					data[data_counter].texture = iter->second[i].m_texture_coords[j];
					data_counter++;
				}
			}

			m_vbos[vbo_count].num_vertices = iter->second.size () * 3;
			m_vbos[vbo_count].material = iter->first;
			m_vbos[vbo_count].vbo.load ((void *)&data[0], m_sizeof_render_data * iter->second.size () * 3, context_id);
		}

		/**
		  * Copy triangles to a VBO using RenderData2 which uses tangents for each triangle.
		  * @param iter Current VBO list to populate.
		  * @param vbo_count Index of VBO being populated.
		  * @param context_id Context Id for the current graphics context.
		  */
		void populateVBO2 (TriangleIterator &iter, int &vbo_count, int context_id)
		{
			m_sizeof_render_data = sizeof (RenderData2); // different

			std::vector<RenderData2> data; // different

			data.resize (iter->second.size () * 3);
			int data_counter = 0;
			for (size_t i = 0; i < iter->second.size (); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					data[data_counter].normal = iter->second[i].m_normals[j];
					data[data_counter].vertex = iter->second[i].m_vertices[j];
					data[data_counter].texture = iter->second[i].m_texture_coords[j];

					// different
					data[data_counter].tangent = iter->second[i].m_tangents[j];

					data_counter++;
				}
			}

			m_vbos[vbo_count].num_vertices = iter->second.size () * 3;
			m_vbos[vbo_count].material = iter->first;
			m_vbos[vbo_count].vbo.load ((void *)&data[0], m_sizeof_render_data * iter->second.size () * 3, context_id);
		}

		/**
		  * Apply a material.
		  * @param material Material to apply.
		  */
		void applyMaterial (const Material *material)
		{
			setMaterial(GL_AMBIENT, material->transmissive);
			setMaterial(GL_DIFFUSE, material->diffuse);
			setMaterial(GL_SPECULAR, material->specular);
			setMaterial(GL_SHININESS, material->specular_exponent);
			
			math::vec3f alpha_ior;
			alpha_ior.x() = material->alpha;
			alpha_ior.y() = material->index_of_refraction;

			glColor3fv (material->diffuse.v);
		}

		/**
		  * Set the current OpenGL state to use this material.
		  * @param param OpenGL parameter to set.
		  * @param v Value to set for the current parameter.
		  */
		inline void setMaterial (GLenum param, const math::vec3f &v)
		{
			GLfloat q[4];
			q[0] = v.x();
			q[1] = v.y();
			q[2] = v.z();
			q[3] = 1.0;
			glMaterialfv(GL_FRONT_AND_BACK, param, q);
		}

		// Data for rendering to a VBO.
		struct RenderData
		{
			math::vec3f vertex;
			math::vec3f normal;
			math::vec2f texture;
		};

		// Data for rendering to a VBO which includes triangle tangents.
		struct RenderData2
		{
			math::vec3f vertex;
			math::vec3f normal;
			math::vec2f texture;
			math::vec4f tangent;
		};

		// Data stored per vbo.
		struct VBOData
		{
			gfx::VertexBuffer vbo;	// VBO to render.
			size_t num_vertices;	// Number of vertices in the VBO to render.
			Material *material;		// Material attached to this VBO.
		};

		// Member variables.
		std::map <std::string, Material> 	m_materials; 		  // List of materials read from the file.
		std::vector <math::Vector <T, 3> >	m_vertices; 		  // Vertices read from the file.
		std::vector <math::Vector <T, 3> >	m_normals;	 	      // Normals read from the file.
		std::vector <math::Vector <T, 2> >	m_texture_coords;	  // Texture coordinates of the mesh.
		size_t                              m_sizeof_render_data; // Size of the render data structure being used (either RenderData or RenderData2)
		bool							 	m_use_normals;		  // Flag to determine if normals should be used or not;
		bool							 	m_use_texture;		  // Flag to determine if tex coords should be used or not;
		bool                                m_use_materials;      // Set GL state to use materials when rendering
		bool                                m_use_tangents;       // Calculate and include tangents in the VBOs
		std::vector <VBOData> 				m_vbos;				  // VBOs created for this model.  Each material spawns a new VBO.
		gfx::ContextBuffer<GLint>			m_tangent_loc;        // Location of the attribute for tangents in a GLSL program

};

template <class T>
void load(Mesh<T> &mesh, const char* filename, bool create_vbo = false)
{
	if (!mesh.load(filename, create_vbo))
	{
		std::cout << "Error loading " << filename << std::endl;
	}
}

}
