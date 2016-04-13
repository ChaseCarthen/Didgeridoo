/*
   Filename : World.h
   Author   : Cody White
   Version  : 1.0

   Purpose  : Controller. 

   Change List:

      - 12/20/2009  - Created (Cody White)

*/

#pragma once

//#include <gfx.h>
#include <Mesh.h>
#include <Skybox.h>
//#include <math/Vector.h>
#include <AL/alut.h>
#include <AL/al.h>
//#include <hydra/Transform.h>
#include <cavr/cavr.h>
#include <Transform.h>

using namespace cavr;

class World
{
	public:

		World (void);

		~World (void);

		void update (const float dt);

		void render (int context_id);

		void initContext (int context_id);

		void destroyContext (int context_id);

		void renderAudio (void);

		void initAudio (void);

	private:

		gfx::Mesh <float> m_didge;
		gfx::Skybox m_skybox;
		bool m_play_sound;
		float m_pitch_offset;
		cavr::math::vec3f m_sound_pos;
		cavr::Transform m_transform;
		int m_sound_index;
		bool m_play_secondary_sound;
		bool m_play_tap;

		struct SoundData
		{
			ALuint buffer;
			ALuint source;
		};
		SoundData* data;
};

