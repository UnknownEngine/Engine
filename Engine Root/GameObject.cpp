#include "GameObject.h"

GameObject::GameObject(std::string name) : active(true), nameID(name), parent(nullptr)
{
}


GameObject::~GameObject()
{
}

void GameObject::Update()
{
}

Component* GameObject::CreateComponent(std::string type, bool active)
{


	return nullptr;
}

void GameObject::ChangeParent(GameObject* newParent)
{
}

void GameObject::DeleteChild(std::string name)
{
}

