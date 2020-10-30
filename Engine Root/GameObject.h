#ifndef _GAMEOBJECTS_H_
#define _GAMEOBJECTS_H_

#include "Globals.h"
#include <vector>
#include <string>

class Component;
class GameObject {

public:
	GameObject(std::string name);
	~GameObject();

	void Update();
	Component* CreateComponent(std::string type, bool active);
	void AddComponent(Component* component);
	void ChangeParent(GameObject* newParent);
	void DeleteChild(std::string name);

public:
	bool active;
	std::string nameID;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	GameObject* parent;
};

#endif