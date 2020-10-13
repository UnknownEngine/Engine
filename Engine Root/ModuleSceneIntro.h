#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	uint my_indices;
	int num_indices;

	int num_vertices;
	uint my_id;
};
