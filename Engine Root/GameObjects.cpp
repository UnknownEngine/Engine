#include "GameObjects.h"

GameObject::GameObject(bool active, std::string name, bool mesh, bool material, bool transform)
{
}

GameObject::~GameObject()
{
}

Components* GameObject::CreateComponent(std::string type, bool active, GameObject* owner)
{
	return nullptr;
}

void GameObject::ChangeParent(GameObject* newParent)
{
}

void GameObject::DeleteChild(std::string name)
{
}

