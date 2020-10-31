#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	TransformComponent(float3 pos, Quat rot, float3 scale);
	~TransformComponent();

	float3 position;
	float3 scale;
	Quat rotation;
};

