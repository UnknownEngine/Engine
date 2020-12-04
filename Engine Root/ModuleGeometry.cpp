#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"
#include "ModuleFSystem.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"


#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Json.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")



struct aiLogStream stream;

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGeometry::~ModuleGeometry()
{
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

bool ModuleGeometry::Start()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	texturesPath = TEXTURES_PATH;
	meshesPath = MESHES_PATH;

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
	if (App->gameMode)
	{
		if (!App->gameModePaused)
		{
			for (int i = 0; i < App->scene_intro->gameObjectsList.size(); i++)
			{
				App->scene_intro->gameObjectsList.at(i)->Update(dt);
			}
		}
	}
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


//  ------------------------------------------FBX LOAD ----------------------------------------------------
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
	gameObject->UID=LCG().Int();
	App->scene_intro->gameObjectsList.push_back(gameObject);

	aiVector3D translation, scaling;
	aiQuaternion rotation;

	scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	//CreateTransformComponent(scene->mRootNode, gameObject);
	//Create child GameObjects for each mesh
	for (uint i = 0; i < scene->mRootNode->mNumChildren; i++)
	{
		aiNode* node = scene->mRootNode->mChildren[i];
		std::string nodeName = node->mName.C_Str();

		bool dummyFound = true;

		while (dummyFound)
		{
			dummyFound = false;

			if (nodeName.find("_$AssimpFbx$_") != std::string::npos && node->mNumChildren == 1) {
				LOG("Has Assimp fbx child");

				node = node->mChildren[0];

				node->mTransformation.Decompose(scaling, rotation, translation);

				
				pos += float3(translation.x, translation.y, translation.z);
				rot = rot * Quat(rotation.x, rotation.y, rotation.z, rotation.w);
				scale = float3(scaling.x, scaling.y, scaling.z);

				nodeName = node->mName.C_Str();
				dummyFound = true;
			}
		}

		TransformComponent* transformComponent = new TransformComponent(pos,rot,scale);
		transformComponent->UID = LCG().Int();
		gameObject->AddComponent(transformComponent);


		GameObject* newGameObject = new GameObject(std::string(node->mName.C_Str()), gameObject);
		newGameObject->ParentUID = gameObject->UID;
		gameObject->childs.push_back(newGameObject);
		CheckNodeChilds(node, newGameObject, scene, realDir);
		
	}
	aiReleaseImport(scene);

	return ret;
}

//  ------------------------------------------Game Object Creation----------------------------------------------------
void ModuleGeometry::CheckNodeChilds(aiNode* node, GameObject* gameObjectNode, const aiScene* scene, std::string realDir)
{
	LOG("Checking node: %s", node->mName.C_Str());

	if (node->mNumMeshes > 0) {
		LOG("I HAVE MESHES!");
		if (node->mNumMeshes > 0) {
			bool ret = false;

			//Get AI data
			MeshComponent* ourMesh = new MeshComponent();
			aiMesh* aimesh = scene->mMeshes[*node->mMeshes];
			ourMesh->name = node->mName.C_Str();
			ourMesh->UID = LCG().Int();
			ourMesh->path = realDir;

			//Copy Vertices
			LoadVertices(aimesh, ourMesh);

			//Copy Faces
			ret = CheckAndLoadFaces(aimesh, ourMesh);

			//Copy Textures
			ret = CheckAndLoadTexCoords(aimesh, ourMesh);

			//Copy Normals
			ret = CheckAndLoadNormals(aimesh, ourMesh);

			//Create AABB
			ourMesh->CreateAABB();

			//Gets size of mesh and stores attributes on the meshBuffer
			ourMesh->size = GetMeshSize(ourMesh);
			ourMesh->meshBuffer = SaveOurMesh(ourMesh, ourMesh->size);
			
			//Write and read on/from library
			App->fsystem->WriteFile((meshesPath+ourMesh->name).c_str(), ourMesh->meshBuffer, ourMesh->size);
			App->fsystem->ReadFile((meshesPath + ourMesh->name).c_str(), &ourMesh->meshBuffer);

			//Loads mesh attributes from meshBuffer
			LoadOurMesh(ourMesh->meshBuffer, ourMesh);

			//Post Loading
			if (ret)
			{
				gameObjectNode->AddComponent(ourMesh);
				CreateBuffer(ourMesh);
			}

			if (scene->HasMaterials()) {
				//Gets AI data
				aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];
				uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				LOG("s", path.C_Str());

				std::string stringPath = std::string(path.C_Str());
				std::size_t pos = stringPath.find_last_of("\\");

				std::string fileRoute = stringPath.substr(pos + 1);
				std::string rootPath = std::string("Assets/Textures/");
				rootPath = rootPath + fileRoute;

				MaterialComponent* materialComponent = new MaterialComponent;

				if (fileRoute != "")
				{
					materialComponent->name = fileRoute;
					materialComponent->name.erase(materialComponent->name.size() - 4);
				}
				else
				{
					materialComponent->name = "";
				}
					materialComponent->UID = LCG().Int();

					//Creates buffer for the texture and loads attributes 
					CreateTextureBuffer(materialComponent);
					LoadTexture(rootPath.c_str(), materialComponent);

					//Gets size of material and loads attributes into materialBuffer
					materialComponent->size = GetMatSize();
					materialComponent->materialBuffer = SaveOurMaterial(materialComponent, materialComponent->size);

					//Writes and reads from/in materials library
					App->fsystem->WriteFile((texturesPath+materialComponent->name).c_str(), materialComponent->materialBuffer, materialComponent->size);
					App->fsystem->ReadFile((texturesPath + materialComponent->name).c_str(), &materialComponent->materialBuffer);

					//Loads material attributes from materialBuffer
					LoadOurMaterial(materialComponent->materialBuffer, materialComponent, materialComponent->size);

					gameObjectNode->AddComponent(materialComponent);
			}	
		}	 
	}
	CreateTransformComponent(node, gameObjectNode);

	if (node->mNumChildren > 0) {
		GameObject* newGameObject = new GameObject(std::string(node->mChildren[0]->mName.C_Str()),gameObjectNode);
		gameObjectNode->childs.push_back(newGameObject);
		LOG("GOT %d children", node->mNumChildren)
			for (uint i = 0; i < node->mNumChildren; i++)
			{
				CheckNodeChilds(node->mChildren[i], newGameObject, scene, realDir);
			}
	}
}


//  ----------------------------------------------------------------------------------------------
//  --------------------------------------------MESHES--------------------------------------------
//  ----------------------------------------------------------------------------------------------


//  ------------------------------------------VERTEX LOAD-----------------------------------
void ModuleGeometry::LoadVertices(aiMesh* aimesh, MeshComponent* ourMesh)
{
	ourMesh->num_vertices = aimesh->mNumVertices;
	ourMesh->vertices = new float[ourMesh->num_vertices * 3];
	memcpy(ourMesh->vertices, aimesh->mVertices, sizeof(float) * ourMesh->num_vertices * 3);
	LOG("New mesh with %d vertices", ourMesh->num_vertices);
}

//  -------------------------------------INDICES LOAD-----------------------------------
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

//  ------------------------------------NORMALS LOAD-------------------------------------
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

//  ------------------------------------TEXTURE COORDS LOAD------------------------------
bool ModuleGeometry::CheckAndLoadTexCoords(aiMesh* aimesh, MeshComponent* ourMesh)
{
	bool ret = false;
	if (aimesh->HasTextureCoords(0))
	{
		ret = true;
		ourMesh->num_tex_coords = ourMesh->num_vertices;
		ourMesh->tex_coords = new float[ourMesh->num_tex_coords * 2];
		for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
		{
			ourMesh->tex_coords[i * 2] = aimesh->mTextureCoords[0][i].x;
			ourMesh->tex_coords[i * 2 + 1] = aimesh->mTextureCoords[0][i].y;
		}
	}
	return false;
}

//  ---------------------------------MESH BUFFER CREATE-----------------------------------
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


//  ----------------------------------------------------------------------------------------------
//  --------------------------------------------TEXTURES------------------------------------------
//  ----------------------------------------------------------------------------------------------

//  ------------------------------------------TEXTURE LOAD-----------------------------------

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

//  ---------------------------------TEXTURE BUFFER CREATE--------------------------------------------
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

//  ----------------------------------TEXTURE CHECKER CREATE------------------------------------
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

//  ----------------------------------------------------------------------------------------------
//  --------------------------------------------CUSTOM FORMAT-------------------------------------
//  ----------------------------------------------------------------------------------------------


//  ---------------------------------------------SIZE GETTERS-------------------------------------
uint ModuleGeometry::GetMeshSize(MeshComponent* ourMesh)
{
	uint ranges[4];
	return (sizeof(ranges) + (sizeof(uint) * ourMesh->num_indices) + (sizeof(float) * ourMesh->num_vertices * 3) + (sizeof(float) * ourMesh->num_normals * 3) + (sizeof(float) * ourMesh->num_tex_coords * 2));
}

uint ModuleGeometry::GetMatSize()
{
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	return(ilSaveL(IL_DDS, nullptr, 0));
}

//  -----------------------------------------MESH SAVE & LOAD------------------------------------
char* ModuleGeometry::SaveOurMesh(MeshComponent* ourMesh, uint size)
{
	uint ranges[4] ;

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	//Store indices
	bytes = sizeof(uint) * ourMesh->num_indices;
	memcpy(cursor, ourMesh->indices, bytes);
	cursor += bytes;

	//Store vertices
	bytes = sizeof(float) * ourMesh->num_vertices * 3;
	memcpy(cursor, ourMesh->vertices, bytes);
	cursor += bytes;

	//Store normals
	bytes = sizeof(float) * ourMesh->num_normals * 3;
	memcpy(cursor, ourMesh->normals, bytes);
	cursor += bytes;

	//Store tex coords
	bytes = sizeof(float) * ourMesh->num_tex_coords * 2;
	memcpy(cursor, ourMesh->tex_coords, bytes);
	cursor += bytes;
	return buffer;
}

uint ModuleGeometry::LoadOurMesh(char* filebuffer, MeshComponent* ourMesh)
{
	uint ranges[4] = { ourMesh->num_indices,ourMesh->num_vertices,ourMesh->num_normals,ourMesh->num_tex_coords };
	char* cursor = filebuffer;

	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(uint) * ourMesh->num_indices;
	ourMesh->indices = new uint[ourMesh->num_indices];
	memcpy(ourMesh->indices, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * ourMesh->num_vertices * 3;
	ourMesh->vertices = new float[ourMesh->num_vertices * 3];
	memcpy(ourMesh->vertices, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * ourMesh->num_normals * 3;
	ourMesh->normals = new float[ourMesh->num_normals * 3];
	memcpy(ourMesh->normals, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * ourMesh->num_tex_coords * 2;
	ourMesh->tex_coords = new float[ourMesh->num_tex_coords * 2];
	memcpy(ourMesh->tex_coords, cursor, bytes);
	cursor += bytes;

	return 1;
}

//  -----------------------------------MATERIAL SAVE & LOAD-------------------------------------
char* ModuleGeometry::SaveOurMaterial(MaterialComponent* ourMaterial, uint size)
{
	char* buffer;
	ilEnable(IL_FILE_OVERWRITE);

	ILubyte* saveBuffer;

	if (size > 0)
	{
		saveBuffer = new ILubyte[size];
		if (ilSaveL(IL_DDS, saveBuffer, size) > 0)
		{
			buffer = (char*)saveBuffer;
		}
	}
	return buffer;
}

void ModuleGeometry::LoadOurMaterial(char* filebuffer, MaterialComponent* ourMaterial, uint size)
{
	ILuint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	ilLoadL(IL_TYPE_UNKNOWN, (const void*)filebuffer, size);
	ourMaterial->bufferTexture = ilutGLBindTexImage()-1;

	ilDeleteImages(1, &ImageName);
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
	transformComponent->UID = LCG().Int();
	gameObjectNode->AddComponent(transformComponent);
}

//  ----------------------------------------------------------------------------------------------
//  --------------------------------------------DRAWS---------------------------------------------
//  ----------------------------------------------------------------------------------------------

void ModuleGeometry::DrawMeshFromGameObjectRoot(GameObject* gameObject)
{
	if (gameObject == nullptr) return;
	if (gameObject->components.size() > 0) {
		for (uint i = 0; i < gameObject->components.size(); i++)
		{
			TransformComponent* transformComponent = gameObject->GetTransformComponent();
			MeshComponent* mesh = gameObject->GetMeshComponent();
			MaterialComponent* material = gameObject->GetMaterialComponent();

			glPushMatrix();
			transformComponent->transform = float4x4::FromTRS(transformComponent->position, transformComponent->rotation, transformComponent->scale);
			float4x4 globalTransform = transformComponent->transform;
			if (gameObject->parent != nullptr) {
				if (gameObject->parent->GetTransformComponent() != nullptr) {
					float4x4 parentGlobal = gameObject->parent->GetTransformComponent()->transform;
					globalTransform = gameObject->parent->GetTransformComponent()->transform * transformComponent->transform;	
				}
			}
			
			glMultMatrixf((float*)&globalTransform.Transposed());
			DrawMesh(mesh, material);
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

void ModuleGeometry::DrawMesh(MeshComponent* mesh, MaterialComponent* material)
{
	if (mesh == nullptr || !mesh->active) return;

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
	if (material->active) {
		if (material != nullptr) {
			if (!material->useChecker) {
				glBindTexture(GL_TEXTURE_2D, material->bufferTexture);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, bufferCheckerTexture);
			}
		}
	}

	//Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);

	//Drawing
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	if (mesh->showNormalsVertices) {
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);
		float normalLenght = 0.1f;
		for (int i = 0; i < mesh->num_normals * 3; i += 3)
		{
			glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
			glVertex3f(mesh->vertices[i] + mesh->normals[i] * normalLenght, mesh->vertices[i + 1] + mesh->normals[i + 1] * normalLenght, mesh->vertices[i + 2] + mesh->normals[i + 2] * normalLenght);
		}

		glEnd();
		glColor3f(1, 1, 1);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}