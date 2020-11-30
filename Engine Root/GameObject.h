#ifndef _GAMEOBJECTS_H_
#define _GAMEOBJECTS_H_

#include "Globals.h"
#include <vector>
#include <string>

class Component;
class MaterialComponent;
class TransformComponent;
class MeshComponent;
class GameObject {

public:
	GameObject(std::string name, GameObject* parent = nullptr);
	~GameObject();

	void Update();
	Component* CreateComponent(std::string type, bool active);
	void AddComponent(Component* component);
	void ChangeParent(GameObject* newParent);
	void DeleteChild(std::string name);
	bool HasMeshComponent(GameObject*);
	MaterialComponent* GetMaterialComponent();
	TransformComponent* GetTransformComponent();
	MeshComponent* GetMeshComponent();

public:
	bool active;
	uint UID;
	std::string nameID;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	GameObject* parent;
};

#endif