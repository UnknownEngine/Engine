#include "GameObject.h"
#include "Component.h"
#include "MaterialComponent.h"

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

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::ChangeParent(GameObject* newParent)
{
}

void GameObject::DeleteChild(std::string name)
{
}

bool GameObject::HasMeshComponent(GameObject* gameObject)
{	

	if (gameObject->childs.size() > 0) {
		for (uint i = 0; i < gameObject->childs.size(); i++)
		{
			HasMeshComponent(gameObject->childs[i]);

		}
	}

	for (uint i = 0; i < gameObject->components.size(); i++)
	{
		if (gameObject->components[i]->type == ComponentType::Mesh) return true;
	}

	return false;
}

MaterialComponent* GameObject::GetMaterialComponent()
{
	MaterialComponent* materialComponent = nullptr;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == ComponentType::Material) {
			materialComponent = static_cast<MaterialComponent*>(components[i]);
			return materialComponent;
		}
	}
	return nullptr;
}

