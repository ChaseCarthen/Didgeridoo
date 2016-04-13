/*
   Filename : FrameBuffer.h
   Author   : Cody White 
   Version  : 1.0

   Purpose  : Encapsulate OpenGL FBO calls. 

   Change List:

      - 10/29/2009  - Created (Cody White)
*/

#pragma once

#include <GL/glew.h>

#include <gfx/Texture.h>
#include <gfx/ContextBuffer.h>
#include <gfx.h>

#include <iostream>

namespace gfx
{

/**
 * Contains the definition for a framebuffer object in OpenGL.
 */
class FrameBuffer
{
	public:

		/**
		  * Default constructor.
		  */
		FrameBuffer (void)
		{
			m_width  = 0;
			m_height = 0;
		}

		/**
		  * Destructor.
		  */
		~FrameBuffer (void)
		{
			if (valid ())
			{
				gfx::ContextBuffer<ContextData>::ContextBufferIterator iter;
				for (iter = _ids.begin ();
			 		iter != _ids.end ();
			 		++iter)
				{
					ContextData& ids = iter->second;

					if (ids.depth)
					{
						glDeleteRenderbuffersEXT (1, &ids.depth);
					}
					
					if (ids.fbo)
					{
						glDeleteFramebuffersEXT (1, &ids.fbo);
						ids.fbo = 0;
					}

					if (ids.stencil)
					{
						glDeleteRenderbuffersEXT (1, &ids.stencil);
					}
				}
			}
		}

		/**
		  * Initialize the  framebuffer.
		  * @param width The width of the framebuffer to allocate.
		  * @param height The height of the framebuffer to allocate.
		  * @param format The format of the framebuffer (e.g. GL_RGBA).
		  * @param internal_format The internal format of the framebuffer.
		  * @param create_depth Flag to create a depth buffer.
		  * @param create_stencil Flag to create a stencil buffer.
		  */
		bool init (size_t width, size_t height, int format, int internal_format, bool create_depth = false, bool create_stencil = false, int context_id = 0)
		{
			ContextData &ids = _ids[context_id];

			m_width = width;
			m_height = height;

			// Create the texture for this FBO.
			gfx::Texture::Params params;
			params.format		  = format;
			params.internalFormat = internal_format;
			params.wrapS 		  = GL_CLAMP_TO_EDGE;
			params.wrapT 		  = GL_CLAMP_TO_EDGE;
			params.minFilter 	  = GL_LINEAR_MIPMAP_LINEAR;
			params.magFilter 	  = GL_LINEAR;
			m_texture.setParams (params);
			m_texture.blank (width, height, GL_FLOAT, context_id);
			m_texture.createMipmaps (context_id);
			m_texture.bind (context_id);
			checkGLErrors("FrameBuffer::init () - create and bind texture");
		
			// Create the FBO.
			glGenFramebuffersEXT (1, &ids.fbo);
			glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, ids.fbo);

			checkGLErrors("FrameBuffer::init () - create and bind fbo");

			// Attach the texture to this FBO.
			glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture.id (context_id), 0);

			if (create_depth)
			{
				// Create a depth buffer and attach it to this FBO.
				glGenRenderbuffersEXT (1, &ids.depth);
				glBindRenderbufferEXT (GL_RENDERBUFFER_EXT, ids.depth);
				glRenderbufferStorageEXT (GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
				glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, ids.depth);
			}

			if (create_stencil)
			{
				// Create a stencil buffer and attach it to this FBO.
				glGenRenderbuffersEXT (1, &ids.stencil);
				glBindRenderbufferEXT (GL_RENDERBUFFER_EXT, ids.stencil);
				glRenderbufferStorageEXT (GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX8_EXT, width, height);
				glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, ids.stencil);
			}

			checkGLErrors("FrameBuffer::init () - create and bind depth/stencil buffer");

			bool success = checkError ();
			glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
			return success;
		}

		void destroyContext (int context_id)
		{
			ContextData& ids = _ids[context_id];

			if (ids.depth)
			{
				glDeleteRenderbuffersEXT (1, &ids.depth);
			}
			
			if (ids.fbo)
			{
				glDeleteFramebuffersEXT (1, &ids.fbo);
				ids.fbo = 0;
			}

			if (ids.stencil)
			{
				glDeleteRenderbuffersEXT (1, &ids.stencil);
			}
		}

		/**
		  * Bind the frame buffer.
		  */
		void bind (int context_id = 0)
		{
			glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, _ids[context_id].fbo);
			glPushAttrib (GL_ALL_ATTRIB_BITS);
			glViewport (0, 0, m_width, m_height);
		}

		/**
		  * Unbind the framebuffer.
		  */
		void unbind (void)
		{
			glPopAttrib ();
			glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);
		}

		/**
		  * Bind the internal texture.
		  */
		void bindTexture (int context_id = 0)
		{
			m_texture.bind (context_id);
		}

		/**
		  * Get the texture id for this framebuffer.
		  */
		GLuint getTexture (int context_id = 0)
		{
			return m_texture.id (context_id);
		}

		/**
		  * Unbind the internal texture.
		  */
		void unbindTexture (void)
		{
			m_texture.unbind ();
		}

		/**
		  * Draw to a level of the mipmap for the texture attached to the framebuffer object.
		  * @param level Level of the mipmap to draw to.
		  */
		void drawToTextureMipmapLevel (int level, int context_id = 0)
		{
			glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture.id (context_id), level);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, level - 1);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level - 1);
			m_texture.bind (context_id);
		}

		void doneDrawingMipmaps (void)
		{
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
		}

		/**
		  * Show error.
		  */
		bool checkError (void)
		{
			switch (glCheckFramebufferStatusEXT (GL_FRAMEBUFFER_EXT))
			{
				case GL_FRAMEBUFFER_COMPLETE_EXT:
				{
					return true;
				}
				break;

				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				{
					std::cout << "FrameBuffer Error: Missing required image/buffer attachment" << std::endl;
				}
				break;

				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				{
					std::cout << "FrameBuffer Error: No buffers attached" << std::endl;
				}
				break;

				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				{
					std::cout << "FrameBuffer Error: Mismatched dimensions" << std::endl;
				}
				break;

				case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				{
					std::cout << "FrameBuffer Error: Colorbuffers have different formats" << std::endl;
				}
				break;

				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				{
					std::cout << "FrameBuffer Error: Trying to draw to non-attached color buffer" << std::endl;
				}
				break;

				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				{
					std::cout << "FrameBuffer Error: Trying to read from non-attached read buffer" << std::endl;
				}
				break;

				case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				{
					std::cout << "FrameBuffer Error: Framebuffers not supported by graphics hardware" << std::endl;
				}
				break;
			}

			return false;
		}

		/**
		  * Check fbo validity.
		  */
		bool isValid (void)
		{
			return valid();
		}

		/**
		 * Returns whether the frame buffer object has been generated.
		 */
		bool valid (void) { return _ids.size (); }

	private:

		struct ContextData
		{
			ContextData()
			{
				fbo = 0;
				depth = 0;
				stencil = 0;
			}

			GLuint fbo;
			GLuint depth;
			GLuint stencil;
		};

		gfx::ContextBuffer<ContextData> _ids;

		size_t m_width;
		size_t m_height;
		
		gfx::Texture m_texture;
};

}
