#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(int uid, ResourceType type): Resource(uid, type)
{
	UID = uid;
	TYPE = type;
}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadToMemory()
{
	
	return false;
}

void ResourceTexture::Save(JsonObj& config) const
{
}

void ResourceTexture::Load(JsonObj& config)
{
}

ResourceType ResourceTexture::GetType() const
{
	return TYPE;
}
