#pragma once

#include "Component.h"
#include "MaterialComponent.h"
#include "MathGeoLib/include/Geometry/AABB.h"

#include "ResourceMesh.h"

class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent();
public:

	uint UID;

	ResourceMesh* r_mesh = nullptr;
};

