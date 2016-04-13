/*
   Filename : World.cpp
   Author   : Cody White
   Version  : 1.0

   Purpose  : Controller. 

   Change List:

      - 12/20/2009  - Created (Cody White)

*/

#include "World.h"


#define MAX_SOUNDS 3

World::World (void)
{
	m_didge.load ("./models/didgeridoo.obj", false);
	m_skybox.load ("./images/skybox");
	m_play_sound = false;
	m_pitch_offset = 0.0f;
	m_sound_index = 1;
	m_play_secondary_sound = false;
	m_play_tap = false;
}

World::~World (void)
{
}

void World::update (const float dt)
{
	static float delay = 0.0f;
	delay -= dt;

	const input::SixDOF *wand = input::getSixDOF ("wand");
	m_play_sound = input::getButton ("button0")->pressed ();
	m_pitch_offset = input::getAnalog ("yAxis")->getValue () * (input::getButton ("button1")->pressed () ? 0.5f : 0.1f);
	m_sound_pos = m_transform.toRealPoint (vec4f(wand->getPosition () + wand->getForward (),1.0)).xyz;
	if (input::getButton ("button5")->pressed () && delay <= 0.0f)
	{
		m_play_secondary_sound = true;
		delay = 0.5f;
	}

	if (fabs (input::getAnalog ("xAxis")->getValue ()) > 0.125f && delay <= 0.0f)
	{
		m_play_tap = true;
		delay = 0.25f;
	}

	m_pitch_offset += input::getButton ("button2")->pressed () ? 0.05f : 0.0f;
	m_pitch_offset -= input::getButton ("button3")->pressed () ? 0.2f : 0.0f;
}

void World::render (int context_id) 
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,1);
	glMatrixMode (GL_MODELVIEW);

	const input::SixDOF *wand = input::getSixDOF ("wand");
	const input::SixDOF *head = input::getSixDOF ("head");
	m_skybox.setPosition (cavr::math::vec3f(head->getPosition ()).v, context_id);
	m_skybox.render (context_id);
	glPushMatrix ();
		glMultMatrixf (cavr::math::mat4f(wand->getMatrix ()).v);
		cavr::math::vec4f light_pos = m_transform.toVirtualPoint (vec4f(head->getPosition (),1.0 ) );
		light_pos[3] = 1.0f;
		glLightfv (GL_LIGHT0, GL_POSITION, light_pos.v);
		m_didge.render (context_id);
	glPopMatrix ();
}

void World::initContext (int context_id)
{
	m_didge.initializeTextures (context_id);
	m_didge.createVBO (context_id);

	m_skybox.initContext (context_id);
}

void World::destroyContext (int context_id)
{
	m_skybox.destroyContext (context_id);
	m_didge.destroyContext (context_id);
}

string getALErrorString(int err) {
  switch (err) {
    case AL_NO_ERROR:
      return "AL_NO_ERROR";
    case AL_INVALID_NAME:
      return "AL_INVALID_NAME";
    case AL_INVALID_ENUM:
      return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE:
      return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION:
      return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY:
      return "AL_OUT_OF_MEMORY";
    default:
      return "No such error code";
  }
}

void World::initAudio (void)
{
	if (alGetError () != AL_NO_ERROR)
	{
		std::cout << getALErrorString(alGetError ()) << endl;
		System::shutdown ();
	}

	alListenerfv (AL_VELOCITY, cavr::math::vec3f().v);
	alDistanceModel (AL_INVERSE_DISTANCE);

	data = new SoundData[MAX_SOUNDS];
	data[0].buffer = alutCreateBufferFromFile ("./sounds/didg1b.wav");
	alGenSources (1, &(data[0].source));
	alSourcei (data[0].source, AL_BUFFER, data[0].buffer);
	alSourcef (data[0].source, AL_GAIN, 4.0f);
	alSourcei (data[0].source, AL_LOOPING, AL_TRUE);
	if (alGetError () != AL_NO_ERROR)
	{
		std::cout << getALErrorString(alGetError ()) << endl;
		std::cout << "Unable to load didg1b.wav" << std::endl;
		System::shutdown ();
	}

	data[1].buffer = alutCreateBufferFromFile ("./sounds/bird.wav");
	alGenSources (1, &(data[1].source));
	alSourcei (data[1].source, AL_BUFFER, data[1].buffer);
	alSourcef (data[1].source, AL_GAIN, 1.0f);
	alSourcei (data[1].source, AL_LOOPING, AL_FALSE);
	if (alGetError () != AL_NO_ERROR)
	{
		std::cout << "Unable to load bird.wav" << std::endl;
		System::shutdown ();
	}

	data[2].buffer = alutCreateBufferFromFile ("./sounds/tap.wav");
	alGenSources (2, &(data[2].source));
	alSourcei (data[2].source, AL_BUFFER, data[2].buffer);
	alSourcef (data[2].source, AL_GAIN, 1.0f);
	alSourcei (data[2].source, AL_LOOPING, AL_FALSE);
	if (alGetError () != AL_NO_ERROR)
	{
		std::cout << "Unable to load tap.wav" << std::endl;
		System::shutdown ();
	}

	//System::setContextData (data);
}

void World::renderAudio (void)
{
	
	//SoundData *data = (SoundData *)System::getContextData();
	float pitch = 1.0f + m_pitch_offset;
	const input::SixDOF *head = input::getSixDOF ("head");
	alListenerfv (AL_POSITION, cavr::math::vec3f(head->getPosition ()).v);
	ALenum state;
	if (m_play_sound)
	{		
		alGetSourcei (data[0].source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alSourcePlay (data[0].source);
		}

		alSourcefv (data[0].source, AL_PITCH, &pitch);
		alSourcefv (data[0].source, AL_POSITION, m_sound_pos.v);
		alSourcefv (data[m_sound_index].source, AL_PITCH, &pitch);
		alSourcefv (data[m_sound_index].source, AL_POSITION, m_sound_pos.v);
	}
	else
	{
		alSourceStop (data[0].source);
	}

	if (m_play_tap)
	{
		m_play_tap = false;
		alSourcePlay (data[2].source);
	}

	alSourcefv (data[2].source, AL_PITCH, &pitch);
	alSourcefv (data[2].source, AL_POSITION, m_sound_pos.v);

	if (m_play_secondary_sound)
	{
		alGetSourcei (data[m_sound_index].source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alSourcePlay (data[m_sound_index].source);
		}

		m_play_secondary_sound = false;
	}
	
	alSourcefv (data[m_sound_index].source, AL_PITCH, &pitch);
	alSourcefv (data[m_sound_index].source, AL_POSITION, m_sound_pos.v);
	
}
