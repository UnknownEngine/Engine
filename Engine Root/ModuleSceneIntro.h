#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"


#include <gl/GL.h>
#include <gl/GLU.h>

#include "GameObject.h"

struct MeshComponent;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	GameObject* selected=nullptr;
	GameObject* CreateGameObject(std::string name);
	std::vector<GameObject*> gameObjectsList;

};
