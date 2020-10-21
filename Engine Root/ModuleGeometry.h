#pragma once

#ifndef __ModuleGeometry_H__
#define __ModuleGeomtry_H__

#include "Module.h"

struct Mesh {
	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;
};

class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool LoadFbx(const char* path);

	bool CleanUp();
	std::vector<Mesh*> ourMeshes;

};

#endif // !__ModuleGeometry_H__


