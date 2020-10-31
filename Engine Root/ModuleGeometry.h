#pragma once

#ifndef __ModuleGeometry_H__
#define __ModuleGeomtry_H__

#include "Module.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )




class MeshComponent;
class aiMesh;
class aiNode;
class aiScene;
class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	
	bool LoadFbx(const char* path, int size, std::string fileName);
	bool LoadTexture(const char* path, MeshComponent* mesh);
	void CreateBuffer(MeshComponent* mesh);

	void LoadVertices(aiMesh* aimesh, MeshComponent* ourMesh);
	bool CheckAndLoadFaces(aiMesh* aimesh, MeshComponent* ourMesh);
	bool CheckAndLoadNormals(aiMesh* aimesh, MeshComponent* ourMesh);
	bool CheckAndLoadTexCoords(aiMesh* aimesh, MeshComponent* ourMesh);

	void RenderMeshes();
	void DrawMeshFromGameObjectRoot(GameObject* gameObject);
	void DrawMesh(MeshComponent* mesh);
	bool CleanUp();
	GameObject* firstGameObject = nullptr;

	void CheckNodeChilds(aiNode* node, GameObject* gameObjectNode, const aiScene* scene);
};

#endif // !__ModuleGeometry_H__


