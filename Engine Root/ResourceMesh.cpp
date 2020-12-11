#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(int uid, ResourceType type):Resource(UID, type)
{
	UID = uid;
	TYPE = type;
}

ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadToMemory()
{
	return false;
}

void ResourceMesh::Save(JsonObj& config) const
{
}

void ResourceMesh::Load(JsonObj& config)
{
}

void ResourceMesh::CreateAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices, num_vertices);
}

AABB& ResourceMesh::GetAABB()
{
	return aabb;
}
