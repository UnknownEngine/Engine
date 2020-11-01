#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "MeshComponent.h"
#include "TransformComponent.h"

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

	CreateCheckerTextureBuffer();

	return true;
}

update_status ModuleGeometry::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGeometry::Update(float dt)
{
	for (uint i = 0; i < App->scene_intro->gameObjectsList.size(); i++)
	{
		DrawMeshFromGameObjectRoot(App->scene_intro->gameObjectsList[i]);
	}
	//RenderMeshes();
	return UPDATE_CONTINUE;
}

update_status ModuleGeometry::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGeometry::LoadFbx(const char* buffer,int size, std::string fileName, std::string realDir) 
{
	bool ret = false;

	//Load aiScene from Fbx with Assimp
	const aiScene* scene = aiImportFileFromMemory(buffer,size, aiProcessPreset_TargetRealtime_MaxQuality,nullptr);
	
	if (scene == nullptr) {
		App->editor->AddLog("Error loading scene");
		return ret;
	}
	//Create Root GameObject
	GameObject* gameObject = new GameObject(fileName);
	App->scene_intro->gameObjectsList.push_back(gameObject);


	CreateTransformComponent(scene->mRootNode, gameObject);
	//Create child GameObjects for each mesh
	for (uint i = 0; i < scene->mRootNode->mNumChildren; i++)
	{
		aiNode* node = scene->mRootNode->mChildren[i];
		GameObject* newGameObject = new GameObject(std::string(node->mName.C_Str()));
		gameObject->childs.push_back(newGameObject);
		CheckNodeChilds(node,newGameObject,scene, realDir);

			
	}
	aiReleaseImport(scene);

	return ret;
}

bool ModuleGeometry::LoadTexture(const char* path, MaterialComponent* material)
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
	material->width	 = (int)ilGetInteger(IL_IMAGE_WIDTH);
	material->height = (int)ilGetInteger(IL_IMAGE_HEIGHT);
	material->bpp = (int)ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	material->dataTexture = ilGetData();
	material->path = path;

	glBindTexture(GL_TEXTURE_2D, material->bufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, material->width, material->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, material->dataTexture);

	glBindTexture(GL_TEXTURE_2D, 0);
	ilDeleteImages(1, &ImgId);
	return ret;
}



void ModuleGeometry::CreateBuffer(MeshComponent* mesh)
{
	//Vertices buffer
	mesh->id_vertices = 0;
	glGenBuffers(1, (GLuint*) & (mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	//Normals buffer     
	glGenBuffers(1, (GLuint*) & (mesh->id_normals));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, &mesh->normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//UVs buffer
	mesh->id_coords = 0;
	glGenBuffers(1, (GLuint*) & (mesh->id_coords));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_coords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 2, mesh->tex_coords, GL_STATIC_DRAW);

	//Indices buffer
	mesh->id_indices = 0;
	glGenBuffers(1, ((GLuint*) & (mesh->id_indices)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

}

void ModuleGeometry::CreateTextureBuffer(MaterialComponent* material)
{
	//Texture buffer
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &material->bufferTexture);
	glBindTexture(GL_TEXTURE_2D, material->bufferTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleGeometry::CreateCheckerTextureBuffer()
{
	GLubyte checkerImage[CHECKER_WIDTH][CHECKER_HEIGHT][4];
	for (int i = 0; i < CHECKER_WIDTH; i++) {
		for (int j = 0; j < CHECKER_HEIGHT; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}
	//Texture buffer
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &bufferCheckerTexture);
	glBindTexture(GL_TEXTURE_2D, bufferCheckerTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKER_WIDTH, CHECKER_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleGeometry::LoadVertices(aiMesh* aimesh, MeshComponent* ourMesh)
{
	ourMesh->num_vertices = aimesh->mNumVertices;
	ourMesh->vertices = new float[ourMesh->num_vertices * 3];
	memcpy(ourMesh->vertices, aimesh->mVertices, sizeof(float) * ourMesh->num_vertices * 3);
	LOG("New mesh with %d vertices", ourMesh->num_vertices);
}

bool ModuleGeometry::CheckAndLoadFaces(aiMesh* aimesh, MeshComponent* ourMesh)
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

bool ModuleGeometry::CheckAndLoadNormals(aiMesh* aimesh, MeshComponent* ourMesh)
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

bool ModuleGeometry::CheckAndLoadTexCoords(aiMesh* aimesh, MeshComponent* ourMesh)
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


void ModuleGeometry::DrawMeshFromGameObjectRoot(GameObject* gameObject)
{
	if (gameObject == nullptr) return;
	if (gameObject->components.size() > 0) {
		for (uint i = 0; i < gameObject->components.size(); i++)
		{

			LOG("Drawing %s components", gameObject->nameID.c_str());

			TransformComponent* transformComponent = gameObject->GetTransformComponent();
			MeshComponent* mesh = gameObject->GetMeshComponent();
			MaterialComponent* material = gameObject->GetMaterialComponent();

			glPushMatrix();
			float4x4 transformMatrix = float4x4::FromTRS(transformComponent->position, transformComponent->rotation, transformComponent->scale);
			glMultMatrixf((float*)&transformMatrix);
			DrawMesh(mesh,material);
			glPopMatrix();
		}

	}
	if (gameObject->childs.size() > 0) {
		for (uint i = 0; i < gameObject->childs.size(); i++)
		{
			DrawMeshFromGameObjectRoot(gameObject->childs[i]);
		}
	}

}

void ModuleGeometry::DrawMesh(MeshComponent* mesh,MaterialComponent* material)
{
	if (mesh == nullptr) return;

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
	if (material != nullptr && material->active) {
		if (!material->useChecker) {
			glBindTexture(GL_TEXTURE_2D, material->bufferTexture);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, bufferCheckerTexture);
		}
	}

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

bool ModuleGeometry::CleanUp()
{
	aiDetachAllLogStreams();

	//for (uint i = 0; i < ourMeshes.size(); i++)
	//{
	//	delete ourMeshes[i];
	//}

	return true;
}

void ModuleGeometry::CheckNodeChilds(aiNode* node,GameObject* gameObjectNode,const aiScene* scene, std::string realDir)
{
	LOG("Checking node: %s", node->mName.C_Str());
	if (node->mNumMeshes > 0) {
		LOG("I HAVE MESHES!");
		bool ret = false;
		if (node->mNumMeshes > 0) {
			MeshComponent* ourMesh = new MeshComponent();
			aiMesh* aimesh = scene->mMeshes[*node->mMeshes];
			ourMesh->name = node->mName.C_Str();
			ourMesh->path = realDir;
			//Copy Vertices
			LoadVertices(aimesh, ourMesh);

			//Copy Faces
			ret = CheckAndLoadFaces(aimesh, ourMesh);

			//Copy Textures
			ret = CheckAndLoadTexCoords(aimesh, ourMesh);

			//Copy Normals
			ret = CheckAndLoadNormals(aimesh, ourMesh);
			if (ret)
			{
				gameObjectNode->AddComponent(ourMesh);
				CreateBuffer(ourMesh);
			}
			if (scene->HasMaterials()) {
				aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];
				uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				std::string stringPath = std::string(path.C_Str());
				std::size_t pos = stringPath.find_last_of("\\");
				
				std::string fileRoute = stringPath.substr(pos +1);
				std::string rootPath = std::string("Assets/Textures/");
				rootPath = rootPath + fileRoute;
				LOG("%s", rootPath.c_str());
				MaterialComponent* materialComponent = new MaterialComponent;
				CreateTextureBuffer(materialComponent);
				LoadTexture(rootPath.c_str(),materialComponent);
				gameObjectNode->AddComponent(materialComponent);

			}
		}
	
	}

	CreateTransformComponent(node, gameObjectNode);

	//Create texture buffer 
	//CreateTextureBuffer();

	
	if (node->mNumChildren > 0) {
		GameObject* newGameObject = new GameObject(std::string(node->mName.C_Str()));
		gameObjectNode->childs.push_back(newGameObject);
		LOG("GOT %d children", node->mNumChildren)
		for (uint i = 0; i < node->mNumChildren; i++)
		{
			CheckNodeChilds(node->mChildren[i],newGameObject,scene, realDir);
		}
	}
}

void ModuleGeometry::CreateTransformComponent(aiNode* node, GameObject* gameObjectNode)
{
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);
	TransformComponent* transformComponent = new TransformComponent(
		float3(translation.x, translation.y, translation.z),
		Quat(rotation.x, rotation.y, rotation.z, rotation.w),
		float3(scaling.x, scaling.y, scaling.z));

	gameObjectNode->AddComponent(transformComponent);
}
