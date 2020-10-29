#ifndef _GAMEOBJECTS_H_
#define _GAMEOBJECTS_H_

#include "Globals.h"
#include <vector>
#include <string>

#include "ModuleSceneIntro.h"
#include "ModuleGeometry.h"

class Components;
class GameObject {

public:
	GameObject(bool active, std::string name, bool mesh, bool material, bool transform);
	~GameObject();
	Components* CreateComponent(std::string type, bool active, GameObject* owner);
	void ChangeParent(GameObject* newParent);
	void DeleteChild(std::string name);

public:
	bool active;
	std::string nameID;
	std::vector<Components*> components;
	std::vector<GameObject*> childs;
	GameObject* parent;
};

#endif