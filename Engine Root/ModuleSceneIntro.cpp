#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	float vertices[] = {
	0.f,0.f,0.f,
	1.f,0.f,0.f,
	1.f,1.f,0.f,
	0.f,1.f,0.f,
	1.f,0.f,-1.f,
	1.f,1.f,-1.f,
	0.f,1.f,-1.f,
	0.f,0.f,-1.f,


	};


	uint indices[] = {
	0,1,2,
	2,3,0,
	1,4,5,
	5,2,1,
	5,6,3,
	3,2,5,
	5,4,7,
	7,6,5,
	7,0,3,
	3,6,7,
	1,0,4,
	0,7,4,
	};

	num_indices = 36;
	my_indices = 0;
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);


	num_vertices = 8;
	my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	
	Planetest p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	if (App->renderer3D->gl_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//DIRECT MODE =======================================
	//glBegin(GL_TRIANGLES); 
	//// front face =================
	//glVertex3f(0.f,0.f,0.f);    // v0-v1-v2
	//glVertex3f(0.f,1.f,0.f);
	//glVertex3f(1.f,1.f,0.f);

	//glVertex3f(0.f,0.f,0.f);    // v2-v3-v0
	//glVertex3f(1.f,0.f,0.f);
	//glVertex3f(1.f,1.f,0.f);

	//// right fce =================
	//glVertex3f(1.f,0.f,0.f);    // v0-v3-v4
	//glVertex3f(1.f,1.f,0.f);
	//glVertex3f(1.f,0.f,1.f);

	//glVertex3f(1.f,1.f,0.f);    // v4-v5-v0
	//glVertex3f(1.f,1.f,1.f);
	//glVertex3f(1.f,0.f,1.f);

	//// top fac ===================
	//glVertex3f(0.f,1.f,0.f);    // v0-v5-v6
	//glVertex3f(1.f,1.f,0.f);
	//glVertex3f(0.f,1.f,1.f);
	//

	//glVertex3f(0.f,1.f,1.f);
	//glVertex3f(1.f,1.f,0.f);    // v6-v1-v0
	//glVertex3f(1.f,1.f,1.f);

	////Left Face =================

	//glVertex3f(0.f, 0.f, 0.f);    
	//glVertex3f(0.f, 0.f, 1.f);
	//glVertex3f(0.f, 1.f, 1.f);


	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 1.f, 0.f);    
	//glVertex3f(0.f, 1.f, 1.f);

	////Back Face =============

	//glVertex3f(0.f, 0.f, 1.f);    
	//glVertex3f(1.f, 0.f, 1.f);
	//glVertex3f(1.f, 1.f, 1.f);


	//glVertex3f(0.f, 0.f, 1.f);
	//glVertex3f(0.f, 1.f, 1.f);   
	//glVertex3f(1.f, 1.f, 1.f);

	////Down Face =============

	//glVertex3f(0.f, 0.f, 0.f);    // v0-v5-v6
	//glVertex3f(1.f, 0.f, 0.f);
	//glVertex3f(1.f, 0.f, 1.f);


	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 0.f, 1.f);    // v6-v1-v0
	//glVertex3f(1.f, 0.f, 1.f);
	//	glEnd();



	//float vertices[108] = { 0.f,0.f,0.f,
	//						0.f,1.f,0.f,
	//						1.f,1.f,0.f,

	//						0.f,0.f,0.f,
	//						1.f,0.f,0.f,
	//						1.f,1.f,0.f,


	//						1.f,0.f,0.f,
	//						1.f,1.f,0.f,
	//						1.f,0.f,1.f,

	//						1.f,1.f,0.f,
	//						1.f,1.f,1.f,
	//						1.f,0.f,1.f,


	//						0.f,1.f,0.f,
	//						1.f,1.f,0.f,
	//						0.f,1.f,1.f,

	//						0.f,1.f,1.f,
	//						1.f,1.f,0.f,
	//						1.f,1.f,1.f,


	//						0.f, 0.f, 0.f,
	//						0.f, 0.f, 1.f,
	//						0.f, 1.f, 1.f,

	//						0.f, 0.f, 0.f,
	//						0.f, 1.f, 0.f,
	//						0.f, 1.f, 1.f,


	//						0.f, 0.f, 1.f,
	//						1.f, 0.f, 1.f,
	//						1.f, 1.f, 1.f,

	//						0.f, 0.f, 1.f,
	//						0.f, 1.f, 1.f,
	//						1.f, 1.f, 1.f,


	//						0.f, 0.f, 0.f,
	//						1.f, 0.f, 0.f,
	//						1.f, 0.f, 1.f,

	//						0.f, 0.f, 0.f,
	//						0.f, 0.f, 1.f,
	//						1.f, 0.f, 1.f
	//};
	//int num_vertices = 36;
	//uint my_id = 0;
	//glGenBuffers(1, (GLuint*) & (my_id));
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	//glDisableClientState(GL_VERTEX_ARRAY);

	//float vertices[] = {
	//0.f,0.f,0.f,
	//1.f,0.f,0.f,
	//1.f,1.f,0.f,

	////1.f,1.f,0.f,
	////0.f,1.f,0.f,
	////0.f,0.f,0.f,

	////1.f,0.f,0.f,
	////1.f,0.f,-1.f,
	////1.f,1.f,-1.f,

	////1.f,1.f,-1.f,
	////1.f,1.f,0.f,
	////1.f,0.f,0.f,
	////
	////1.f,1.f,0.f,
	////1.f,1.f,-1.f,
	////0.f,1.f,0.f,

	////1.f,1.f,-1.f,
	////0.f,1.f,-1.f,
	////0.f,1.f,0.f,

	//};




	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}


