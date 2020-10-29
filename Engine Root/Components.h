#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include "Globals.h"
#include <vector>


#include "ModuleSceneIntro.h"
#include "ModuleGeometry.h"

class GameObject;
enum ComponentType {
	Mesh,
	Material,
	Transform,
};

class Components {
	virtual void Enable();
	virtual void Update();
	virtual void Disable();
public:
	ComponentType type;
	bool active;
	GameObject* owner;

};

#endif
