#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"


#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

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

	/*uint indices_array[2520];
	float vertices_array[1260];*/

	uint* indices_array;
	float* vertices_array;
	GLuint bufferTexture = 0;
	ILubyte* dataTexture = nullptr;
	std::vector<float> vertices;
	std::vector<uint> indices;
	std::vector<uint> textcoords;
};
