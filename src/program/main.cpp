/*
   Filename : main.cpp
   Author   : Cody White
   Version  : 1.0

   Purpose  : Main driver. 

   Change List:

      - 12/20/2009  - Created (Cody White)

*/
#include <cavr/cavr.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <AL/alut.h>
#include <AL/al.h>
#include "World.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glx.h>

using namespace cavr;

World world;

//const Button *exit_button;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat mat_diffuse[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

void alrender (void)
{
	world.renderAudio ();
}

void alinit (void)
{
	world.initAudio ();
}

//#define cavr::System System
int counter = 0;
void update (void)
{
	if (cavr::input::getButton("exit")->pressed ())
	{
		System::shutdown ();
		return;
	}

	float dt = cavr::input::InputManager::dt();
	world.update (dt);
}

void render (void)
{
	glClearColor(1,1,1,1);
	alrender();
	world.render (1);
}



void initContext (void)
{
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glEnable (GL_DEPTH_TEST);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glewInit ();

	// Lighting.
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable (GL_COLOR_MATERIAL);
	glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	

	world.initContext (1);
	cavr::System::setContextData(&world);
}

void destroyContext (void)
{
	world.destroyContext (1);
}

int main (int argc, char **argv)
{
	ALCdevice *dev;
    ALCcontext *ctx;

    dev = alcOpenDevice(NULL);
    ctx = alcCreateContext(dev, NULL);
    alcMakeContextCurrent(ctx);

	System::setCallback ("update_gl_context", update);
	System::setCallback ("gl_render", render);
	System::setCallback ("init_gl_context", initContext);
	System::setCallback ("destruct_gl_context", destroyContext);
	System::setCallback ("alrender", alrender);
	System::setCallback ("alinit", alinit);

	cavr::input::InputMap input_map;

	// sixdofs
	input_map.sixdof_map["wand"] = "vrpn[WiiMote0[0]]";
	input_map.sixdof_map["head"] = "vrpn[ShortGlasses[0]]";

	// buttons
	input_map.button_map["button0"] = "vrpn[WiiMote0[3]]"; // playing primary sound -- A
	input_map.button_map["button1"] = "vrpn[WiiMote0[16]]"; // pitch offset multiplier on means a higher multiplier -- z button nunchaku
	input_map.button_map["button2"] = "vrpn[WiiMote0[1]]"; // pitch up -- 1
	input_map.button_map["button3"] = "vrpn[WiiMote0[2]]"; // pitch down -- 2
	input_map.button_map["button5"] = "vrpn[WiiMote0[4]]"; // play secondary sound?! -- B
	input_map.button_map["exit"] = "vrpn[WiiMote0[0]]"; // play secondary sound?! -- B

	//analog
	input_map.analog_map["xAxis"] = "vrpn[WiiMote0[21]]"; // axis on nunchaku
	input_map.analog_map["yAxis"] = "vrpn[WiiMote0[22]]"; // axis on nunchaku

	alutInitWithoutContext (&argc, argv);
	alinit();
	if (!System::init (argc, argv, &input_map))
	{
		cout << "EXITING NOW!" << endl;
		exit (1);
	}
	//exit_button = Input::getButton ("exit");

	System::run ();
}
