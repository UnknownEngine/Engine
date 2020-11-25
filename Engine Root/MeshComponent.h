#pragma once

#include "Component.h"
#include "MaterialComponent.h"
class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent();

	void Update() override;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;

	uint size = 0;
	char* meshBuffer;
	float* normals = nullptr;

	uint id_coords = 0;
	uint num_tex_coords = 0;
	float* tex_coords = nullptr;

	bool showNormalsVertices = false;

	std::string name;
	std::string path;
};

