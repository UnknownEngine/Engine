#pragma once
#include "Component.h"
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"

class CameraComponent : public Component
{
public:
	CameraComponent();
	~CameraComponent();

	void UpdateFrustum(float w, float h);

public:

	Frustum frustum;
	float fov;

};

