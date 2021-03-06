#include "GameObject.h"
#include "Component.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"

GameObject::GameObject(std::string name,int uid, GameObject* parent) : active(true), nameID(name), parent(parent),UID(uid)
{
}


GameObject::~GameObject()
{
}

void GameObject::Update(float dt)
{
	/*for (int i = 0; i<components.size(); i++)
	{
		if (components[i]->type == ComponentType::Transform)
		{
			TransformComponent* transform = new TransformComponent();
			transform = GetTransformComponent();
			transform->position.x += 0.01;
			components[i] = transform;
		}
	}*/
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

TransformComponent* GameObject::GetTransformComponent()
{
	TransformComponent* transformComponent = nullptr;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == ComponentType::Transform) {
			transformComponent = static_cast<TransformComponent*>(components[i]);
			return transformComponent;
		}
	}
	return nullptr;
}

MeshComponent* GameObject::GetMeshComponent()
{
	MeshComponent* meshComponent = nullptr;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == ComponentType::Mesh) {
			meshComponent = static_cast<MeshComponent*>(components[i]);
			return meshComponent;
		}
	}
	return nullptr;
}

CameraComponent* GameObject::GetCameraComponent()
{
	CameraComponent* cameraComponent = nullptr;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == ComponentType::Camera) {
			cameraComponent = static_cast<CameraComponent*>(components[i]);
			return cameraComponent;
		}
	}
	return nullptr;
}

AABB& GameObject::GetAABB()
{
	return aabb;
}

OBB& GameObject::GetOBB()
{
	return obb;
}

void GameObject::UpdateAABB()
{
	MeshComponent* mesh = GetMeshComponent();
	if (mesh)
	{
		//Generate global OBB
		obb = mesh->r_mesh->GetAABB();
		TransformComponent* t = GetTransformComponent();
		obb.Transform(t->global_transform);

		//Generate global AABB
		aabb.SetNegativeInfinity();
		aabb.Enclose(obb);
	}
}

