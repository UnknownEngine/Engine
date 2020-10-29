#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "ModuleSceneIntro.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct aiLogStream stream;

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGeometry::~ModuleGeometry()
{
}

bool ModuleGeometry::Start()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

update_status ModuleGeometry::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGeometry::Update(float dt)
{
	RenderMeshes();
	return UPDATE_CONTINUE;
}

update_status ModuleGeometry::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGeometry::LoadFbx(const char* buffer,int size) 
{
	bool ret = false;
	Mesh* ourMesh = new Mesh;
	const aiScene* scene = aiImportFileFromMemory(buffer,size, aiProcessPreset_TargetRealtime_MaxQuality,nullptr);

	if (scene != nullptr && scene->HasMeshes()) 
	{
		//For all meshes
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* aimesh = scene->mMeshes[i];

			//Copy Vertices
			LoadVertices(aimesh, ourMesh);

			//Copy Faces
			ret = CheckAndLoadFaces(aimesh, ourMesh);

			//Copy Textures
			ret = CheckAndLoadTexCoords(aimesh, ourMesh);

			//Copy Normals
			ret = CheckAndLoadNormals(aimesh, ourMesh);
		}
		if (ret)
		{
			ourMeshes.push_back(ourMesh);
			CreateBuffer(ourMesh);
			aiReleaseImport(scene);
		}
	}
	else 
	{
		App->editor->AddLog("Error loading scene");
	}
	return ret;
}

bool ModuleGeometry::LoadTexture(const char* path, Mesh* mesh)
{
	bool ret = true;
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	const char* filename = path;

	//Name Image Bind
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);

	if (!ilLoadImage(filename))
	{
		ILenum err = ilGetError();
		printf("An error occured while loading %s: %d (%s)\n", filename, err, iluErrorString(err));
		return false;
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	mesh->texture.width	 = (int)ilGetInteger(IL_IMAGE_WIDTH);
	mesh->texture.height = (int)ilGetInteger(IL_IMAGE_HEIGHT);
	mesh->texture.bpp	 = (int)ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	mesh->texture.dataTexture = ilGetData();

	glBindTexture(GL_TEXTURE_2D, mesh->texture.bufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mesh->texture.width, mesh->texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mesh->texture.dataTexture);

	glBindTexture(GL_TEXTURE_2D, 0);
	ilDeleteImages(1, &ImgId);
	return ret;
}



void ModuleGeometry::CreateBuffer(Mesh* mesh)
{
	//Vertices
	mesh->id_vertices = 0;
	glGenBuffers(1, (GLuint*) & (mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	//Normals buffer     
	glGenBuffers(1, (GLuint*) & (mesh->id_normals));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, &mesh->normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Textures
	mesh->id_coords = 0;
	glGenBuffers(1, (GLuint*) & (mesh->id_coords));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_coords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 2, mesh->tex_coords, GL_STATIC_DRAW);

	//Indices
	mesh->id_indices = 0;
	glGenBuffers(1, ((GLuint*) & (mesh->id_indices)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

	//Textures 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mesh->texture.bufferTexture);
	glBindTexture(GL_TEXTURE_2D, mesh->texture.bufferTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleGeometry::LoadVertices(aiMesh* aimesh, Mesh* ourMesh)
{
	ourMesh->num_vertices = aimesh->mNumVertices;
	ourMesh->vertices = new float[ourMesh->num_vertices * 3];
	memcpy(ourMesh->vertices, aimesh->mVertices, sizeof(float) * ourMesh->num_vertices * 3);
	LOG("New mesh with %d vertices", ourMesh->num_vertices);
}

bool ModuleGeometry::CheckAndLoadFaces(aiMesh* aimesh, Mesh* ourMesh)
{
	bool ret = false;
	if (aimesh->HasFaces())
	{
		ret = true;
		ourMesh->num_indices = aimesh->mNumFaces * 3;
		ourMesh->indices = new uint[ourMesh->num_indices];
		for (uint i = 0; i < aimesh->mNumFaces; i++)
		{
			if (aimesh->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else {
				memcpy(&ourMesh->indices[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	return ret;
}

bool ModuleGeometry::CheckAndLoadNormals(aiMesh* aimesh, Mesh* ourMesh)
{
	bool ret = false;
	if (aimesh->HasNormals())
	{
		ret = true;
		ourMesh->num_normals = ourMesh->num_vertices;
		ourMesh->normals = new float[ourMesh->num_normals * 3];
		memcpy(ourMesh->normals, aimesh->mNormals, sizeof(float) * ourMesh->num_normals * 3);
			LOG("New mesh with %d normals", ourMesh->num_normals);
	}
	return ret;
}

bool ModuleGeometry::CheckAndLoadTexCoords(aiMesh* aimesh, Mesh* ourMesh)
{
	bool ret = false;
	if (aimesh->HasTextureCoords(0))
	{
		ret = true;
		ourMesh->tex_coords = new float[aimesh->mNumVertices * 2];
		for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
		{
			ourMesh->tex_coords[i * 2] = aimesh->mTextureCoords[0][i].x;
			ourMesh->tex_coords[i * 2 + 1] = aimesh->mTextureCoords[0][i].y;
		}
	}
	return false;
}

void ModuleGeometry::RenderMeshes()
{
	for (uint i = 0; i < ourMeshes.size(); i++)
	{
		Mesh* mesh = ourMeshes[i];

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//Vertices
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices); 
		glVertexPointer(3, GL_FLOAT, 0, NULL); 

		//normals
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		//UVs
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_coords);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		//Textures
		glBindTexture(GL_TEXTURE_2D, mesh->texture.bufferTexture);
		
		//Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices); 

		//Drawing
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL); 

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

bool ModuleGeometry::CleanUp()
{
	aiDetachAllLogStreams();

	for (uint i = 0; i < ourMeshes.size(); i++)
	{
		delete ourMeshes[i];
	}

	return true;
}
