#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#define MAX_LIGHTS 8
struct Mesh;
class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	void Draw(Mesh* mesh);
	bool CleanUp();

	void OnResize(int width, int height);
	void ActivateCheckBoxs(bool is_active, int identifier);
public:
	bool gl_depth_test = false;
	bool gl_cull_face = false;
	bool gl_lightning = true;
	bool gl_color_material = false;
	bool gl_texture_2d = false;
	bool gl_ambient = false;
	bool gl_ambient_diffuse = false;
	bool gl_wireframe = false;

public:

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};