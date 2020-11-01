#include "TransformComponent.h"


TransformComponent::TransformComponent() : Component()
{
	type = ComponentType::Transform;
	position = float3::zero;
	scale = float3::one;
	rotation = Quat::identity;
}

TransformComponent::TransformComponent(float3 pos, Quat rot, float3 scale) : Component()
{
	type = ComponentType::Transform;
	position = pos;
	rotation = rot;
	this->scale = scale;
}

TransformComponent::~TransformComponent()
{
}
