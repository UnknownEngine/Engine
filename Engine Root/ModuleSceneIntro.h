#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>

struct Mesh;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	void Draw();
	bool CleanUp();

	void PushSphereIndices(std::vector<uint>& indices, int sectors, int r, int s);
	void CreateSphere(std::vector<float>& vertices, std::vector<uint>& indices, float radius, uint rings, uint sectors);

	void CreateBuffer(Mesh* mesh);
public:

	uint my_indices;
	int num_indices;

	uint my_vertices;
	int num_vertices;

	uint indices_array[2520];
	float vertices_array[1260];

	std::vector<float> vertices;
	std::vector<uint> indices;
	std::vector<uint> textcoords;
};
