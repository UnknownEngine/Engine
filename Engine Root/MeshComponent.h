#pragma once

#include "Component.h"
#include "MaterialComponent.h"

class GameObject;
class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent();

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

	MaterialComponent texture;
};

