#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleWindow.h"


#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"


#include <gl/GL.h>
#include <gl/GLU.h>

#define CHECKERS_HEIGHT 8
#define CHECKERS_WIDTH 8

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateGameObject("Empty GameObject");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(std::string name)
{
	GameObject* gameObject = new GameObject(name);

	gameObjectsList.push_back(gameObject);
	LOG("Created GameObject: %s", gameObject->nameID.c_str());
	return gameObject;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	
	Planetest p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	if (App->renderer3D->gl_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	return UPDATE_CONTINUE;
}





