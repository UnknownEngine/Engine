#pragma once
#include "Resource.h"
#include "MathGeoLib/include/Geometry/AABB.h"

class ResourceMesh : public Resource {
public:
	ResourceMesh(int UID);
	~ResourceMesh();
	bool LoadToMemory() override;
	void Save(JsonObj& config) const override;
	void Load(JsonObj& config) override;

	void CreateAABB();
	AABB& GetAABB();

public:
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
	uint UID;

	AABB aabb;


};