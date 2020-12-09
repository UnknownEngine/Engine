#include "Resource.h"
Resource::Resource(int UID, ResourceType type)
{
}

Resource::~Resource()
{
}

ResourceType Resource::GetType() const
{
	return ResourceType();
}

int Resource::GetUID() const
{
	return 0;
}

const char* Resource::GetAssetFile() const
{
	return nullptr;
}

const char* Resource::GetLibraryFile() const
{
	return nullptr;
}

bool Resource::IsLoadedToMemory() const
{
	return false;
}

bool Resource::LoadToMemory()
{
	
	return false;
}

void Resource::Save(JsonObj& config) const
{
}

void Resource::Load(JsonObj& config)
{
}