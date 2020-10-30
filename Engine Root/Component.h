#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include "Globals.h"
#include <vector>



class GameObject;
enum class ComponentType {
	None,
	Mesh,
	Material,
	Transform,
};

class Component {

public:
	Component();
	~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	virtual void OnEditor();
public:
	bool active;
	ComponentType type;
	GameObject* owner;

};

#endif
