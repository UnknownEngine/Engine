#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(int UID):Resource(UID, type)
{
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
}

AABB& ResourceMesh::GetAABB()
{
	// TODO: insertar una instrucción return aquí
	return AABB();
}
