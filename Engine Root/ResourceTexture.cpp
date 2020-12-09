#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(int uid): Resource(uid, type)
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
