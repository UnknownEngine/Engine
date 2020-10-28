#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#define CHECKERS_HEIGHT 8
#define CHECKERS_WIDTH 8

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

	/*float vertices[] = {
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
	
	num_vertices = 8;
	my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);*/


	//std::vector<int> hola;

	//CreateSphere(vertices, indices, 2.f,20, 20);

	//num_indices = indices.size();
	//num_vertices = vertices.size();

	//indices_array = new uint[num_indices];
	//vertices_array = new float[num_vertices];

	//indices_array[0] = 0;
	//indices_array[1] = 0;
	//indices_array[2] = 0;

	//for (int i = 0; i < num_indices; i++)
	//{
	//	indices_array[i] = indices[i];
	//}

	//for (int i = 0; i < num_vertices; i++)
	//{
	//	vertices_array[i] = vertices[i];
	//}


	//my_indices = 0;
	//glGenBuffers(1, ((GLuint*) & (my_indices)));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices ,&indices_array, GL_STATIC_DRAW);

	//my_vertices = 0;
	//glGenBuffers(1, (GLuint*) & (my_vertices));
	//glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, &vertices_array, GL_STATIC_DRAW); 


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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::Draw()
{
	for (uint i = 0; i < App->geometry->ourMeshes.size(); i++)
	{
		//Get mesh
		Mesh* mesh = App->geometry->ourMeshes[i];
		
		//Draw mesh
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices); //Select buffer
		glVertexPointer(3, GL_FLOAT, 0, NULL); //Set vertex


		if (mesh->num_normals != 0) {
			glEnableClientState(GL_NORMAL_ARRAY);   
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals); 
			glNormalPointer(GL_FLOAT, 0, NULL);   
		}
		glBindTexture(GL_TEXTURE_2D, mesh->id_textures);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices); //Select buffer

		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL); // Draw with the last buffer selected

		//glColor3f(1, 0, 0);
		//glBegin(GL_LINES);
		//for (int i = 0; i < mesh->num_normals * 3; i += 3)
		//{
		//	glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
		//	glVertex3f((mesh->vertices[i] + mesh->normals[i]) * .2f, (mesh->vertices[i + 1] + mesh->normals[i + 1]) * .2f, (mesh->vertices[i + 2] + mesh->normals[i + 2])*0.2f);
		//	LOG("%f, %f, %f", mesh->normals[i], mesh->normals[i + 1], mesh->normals[i + 2]);
		//}

		//glColor3f(1, 1, 1);
		//glColor3f(0, 1, 0);  
		//glBegin(GL_LINES); 
		//float normalLenght = 0.05f;   
		//for (int i = 0; i < mesh->num_normals * 3; i += 3) 
		//{ glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
		//glVertex3f(mesh->vertices[i] + mesh->normals[i] * normalLenght, mesh->vertices[i + 1] + mesh->normals[i + 1] * normalLenght, mesh->vertices[i + 2] + mesh->normals[i + 2] * normalLenght);
		//}    
		//glEnd();  
		//glColor3f(1, 1, 1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}	
}

void ModuleSceneIntro::PushSphereIndices(std::vector<uint>& indices, int sectors, int r, int s)
{
	
	int curRow = r * sectors;
	int nextRow = (r + 1) * sectors;

	indices.push_back(curRow + s);
	indices.push_back(nextRow + s);
	indices.push_back(nextRow + (s + 1));

	indices.push_back(curRow + s);
	indices.push_back(nextRow + (s + 1));
	indices.push_back(curRow + (s + 1));
}


void ModuleSceneIntro::CreateSphere(std::vector<float>& vertices, std::vector<uint>& indices, float radius, uint rings, uint sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	for (int r = 0; r < rings; ++r) {
		for (int s = 0; s <= sectors; ++s) {
			float y = sin(-M_PI_2 + M_PI * r * R);
			float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			vertices.push_back(x * radius);
			vertices.push_back(y * radius);
			vertices.push_back(z * radius);
			PushSphereIndices(indices, sectors, r, s);
		}
	}
}

void ModuleSceneIntro::CreateBuffer(Mesh* mesh)
{
	mesh->id_indices = 0;
	glGenBuffers(1, ((GLuint*)&(mesh->id_indices)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

	mesh->id_vertices = 0;
	glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	//mesh->id_normals = 0;
	//glGenBuffers(1, (GLuint*)&(mesh->id_normals));
	//glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);

	//Normals buffer     
	glGenBuffers(1, (GLuint*)&(mesh->id_normals));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, &mesh->normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glEnableVertexAttribArray(2);    
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);  
	
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	//Textures Buffer
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mesh->id_textures);
	glBindTexture(GL_TEXTURE_2D, mesh->id_textures);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);




	

}


