#pragma once

#ifndef __ModuleGeometry_H__
#define __ModuleGeomtry_H__

#include "Module.h"

struct Texture {
	int width = 0;
	int height = 0;
	int bpp = 0;
	GLuint bufferTexture = 0;
	ILubyte* dataTexture = nullptr;
};

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

	uint id_coords = 0;
	float* tex_coords = nullptr;

	Texture texture;
};

class aiMesh;
class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	
	bool LoadFbx(const char* path, int size);
	bool LoadTexture(const char* path, Mesh* mesh);
	void CreateBuffer(Mesh* mesh);

	void LoadVertices(aiMesh* aimesh, Mesh* ourMesh);
	bool CheckAndLoadFaces(aiMesh* aimesh, Mesh* ourMesh);
	bool CheckAndLoadNormals(aiMesh* aimesh, Mesh* ourMesh);
	bool CheckAndLoadTexCoords(aiMesh* aimesh, Mesh* ourMesh);

	void RenderMeshes();

	bool CleanUp();
	std::vector<Mesh*> ourMeshes;

};

#endif // !__ModuleGeometry_H__


