#include "MeshComponent.h"

MeshComponent::MeshComponent() :  Component()
{
	type = ComponentType::Mesh;
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Update()
{
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	////Vertices
	//glBindBuffer(GL_ARRAY_BUFFER,id_vertices);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	////normals
	//glBindBuffer(GL_ARRAY_BUFFER, id_normals);
	//glNormalPointer(GL_FLOAT, 0, NULL);

	////UVs
	//glBindBuffer(GL_ARRAY_BUFFER, id_coords);
	//glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	////Textures
	//glBindTexture(GL_TEXTURE_2D, texture.bufferTexture);

	////Indices
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	////Drawing
	//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void MeshComponent::CreateAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices, num_vertices);
}

AABB& MeshComponent::GetAABB()
{
	return aabb;
}




