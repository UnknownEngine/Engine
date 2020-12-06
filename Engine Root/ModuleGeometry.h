#pragma once

#ifndef __ModuleGeometry_H__
#define __ModuleGeomtry_H__

#include "Module.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

#define CHECKER_HEIGHT 128
#define CHECKER_WIDTH 128


class MeshComponent;
class MaterialComponent;
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
	bool CleanUp();

	bool LoadFbx(const char* path, int size, std::string fileName,std::string realDir);

	void CheckNodeChilds(aiNode* node, GameObject* gameObjectNode, const aiScene* scene, std::string realDir);

	void LoadVertices(aiMesh* aimesh, MeshComponent* ourMesh);
	bool CheckAndLoadFaces(aiMesh* aimesh, MeshComponent* ourMesh);
	bool CheckAndLoadNormals(aiMesh* aimesh, MeshComponent* ourMesh);
	bool CheckAndLoadTexCoords(aiMesh* aimesh, MeshComponent* ourMesh);
	void CreateBuffer(MeshComponent* mesh);

	bool LoadTexture(const char* path, MaterialComponent* gameObject);
	void CreateTextureBuffer(MaterialComponent* material);
	void CreateCheckerTextureBuffer();

	uint  GetMeshSize(MeshComponent* ourMesh);
	char* SaveOurMesh(MeshComponent* ourMesh, uint size);
	uint LoadOurMesh(char* filebuffer, MeshComponent* ourMesh);

	uint GetMatSize();
	char* SaveOurMaterial(MaterialComponent* ourMaterial, uint size);
	void LoadOurMaterial(char* filebuffer, MaterialComponent* ourMaterial, uint size);
	
	void CreateTransformComponent(aiNode* node, GameObject* gameObjectNode);
	void DrawMeshFromGameObjectRoot(GameObject* gameObject);
	void DrawMesh(MeshComponent* mesh, MaterialComponent* material);

	int ContainsAaBox(const AABB& refBox, Frustum* frustum);

	GLuint bufferCheckerTexture = 0;
	std::string texturesPath;
	std::string meshesPath;
private:
};

#endif // !__ModuleGeometry_H__


