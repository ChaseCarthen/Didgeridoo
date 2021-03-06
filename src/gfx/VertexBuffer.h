/*
   Filename : VertexBuffer.h
   Author   : Cody White and Joe Mahsman
   Version  : 1.0

   Change List:

      - 06/18/2009  - Created (Cody White and Joe Mahsman)
*/

#pragma once

#include <GL/glew.h>
#include <ContextBuffer.h>
#include <iostream>

namespace gfx
{

/**
 * Abstracts OpenGL vertex buffer objects.
 */
class VertexBuffer
{
	public:
		
		/** 
		 * Default constructor.
		 * @param target Type of VBO to use.
		 */
		VertexBuffer (GLenum target = GL_ARRAY_BUFFER)
		{
			m_target = target;
		}

		/** 
		 * Default destructor.
		 */
		~VertexBuffer ()
		{
			gfx::ContextBuffer<GLuint>::ContextBufferIterator iter;
			for (iter = m_ids.begin (); iter != m_ids.end (); ++iter)
			{
				glDeleteBuffers (1, &(iter->second));
			}
		}

		/**
		  * Destroy a context.
		  * @param context_id ID of context to destrory.
		  */
		void destroyContext (int context_id)
		{
			glDeleteBuffers (1, &m_ids[context_id]);
			m_ids.remove (context_id);
		}

		/**
		 * Assignment operator.
		 * @param other The vertex buffer to set this object equal to.
		 */
		VertexBuffer & operator= (const VertexBuffer &other)
		{
			if (this != &other)
			{
				gfx::ContextBuffer<GLuint>::ConstContextBufferIterator iter;
				for (iter = other.m_ids.begin (); iter != other.m_ids.end (); ++iter)
				{
					m_ids[iter->first] = iter->second;
				}

				m_target = other.m_target;
			}

			return *this;
		}

		/**
		 * Generate the VBO id and load the data.
		 * @param data The data to populate the VBO with.
		 * @param size Size in bytes of the data.
		 */
		void load (void *data, int size, int context_id = 0)
		{
			GLuint& id = m_ids[context_id];
			glGenBuffers (1, &id);
			
			if (id == 0)
			{
				std::cout << "VertexBuffer::load () - Error: Unable to generate VBO!" << std::endl;
			}

			glBindBuffer (m_target, id);
			glBufferData (m_target, size, data, GL_STATIC_DRAW);
		}

		/**
		 * Bind the VBO for use. Must be called after
		 * load ().
		 */
		void bind (int context_id = 0) const
		{
			glBindBuffer (m_target, m_ids[context_id]);
		}

		/**
		 * Unbind this VBO from use.
		 */
		void unbind () const
		{
			glBindBuffer (m_target, 0);
		}

		/** 
		 * Return the VBO id assigned by OpenGL. 
		 */
		GLuint id (int context_id = 0) const
		{
			return m_ids[context_id];
		}

	private:

		gfx::ContextBuffer <GLuint> m_ids;
		GLenum m_target;
};

}

