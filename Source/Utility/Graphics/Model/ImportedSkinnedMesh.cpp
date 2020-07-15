#include <skelpch.h>
#include "ImportedSkinnedMesh.h"

namespace Skel
{
	// Constructor with custom texture
	ImportedSkinnedMesh::ImportedSkinnedMesh(std::vector<SkinnedVertex> vertexBuffer, std::vector<unsigned int> indexBuffer, Texture texture)
		: m_Vertices(vertexBuffer),
		m_Indicies(indexBuffer),
		Mesh()
	{
		SetBufferLayout(GetBufferLayout());
		SetData(&m_Vertices[0], m_Vertices.size() * sizeof(SkinnedVertex), &m_Indicies[0], static_cast<unsigned int>(m_Indicies.size()), texture, m_Color);
	}

	// Constructor with custom color
	ImportedSkinnedMesh::ImportedSkinnedMesh(std::vector<SkinnedVertex> vertexBuffer, std::vector<unsigned int> indexBuffer, Vector3 color)
		: m_Vertices(vertexBuffer),
		m_Indicies(indexBuffer),
		Mesh()
	{
		SetBufferLayout(GetBufferLayout());
		SetData(&m_Vertices[0], m_Vertices.size() * sizeof(SkinnedVertex), &m_Indicies[0], static_cast<unsigned int>(m_Indicies.size()), m_Texture, color);
	}

	// Copy constructor
	ImportedSkinnedMesh::ImportedSkinnedMesh(const ImportedSkinnedMesh& other)
		: m_Vertices(other.m_Vertices),
		m_Indicies(other.m_Indicies),
		Mesh()
	{
		SetBufferLayout(GetBufferLayout());
		SetData((void*)& m_Vertices[0], m_Vertices.size() * sizeof(SkinnedVertex), &m_Indicies[0], static_cast<unsigned int>(m_Indicies.size()), other.m_Texture, other.m_Color);
	}
	ImportedSkinnedMesh::~ImportedSkinnedMesh()
	{
	}
	VertexBufferLayout ImportedSkinnedMesh::GetBufferLayout()
	{
		VertexBufferLayout vertexBufferLayout;
		vertexBufferLayout.Push<float>(3);			// Position
		vertexBufferLayout.Push<float>(3);			// Normals
		vertexBufferLayout.Push<float>(2);			// Texture Coords
		vertexBufferLayout.Push<unsigned int>(4);	// Bone IDs
		vertexBufferLayout.Push<float>(4);			// Bone weights
		return vertexBufferLayout;
	}
	// Fills in weight associated with bone. Cannot have more than limit (NUM_BONES_PER_VERTEX)
	void SkinnedVertex::AddBoneData(int boneID, float weight)
	{
		for (uint8 i = 0; i < NUM_BONES_PER_VERTEX; i++) {
			if (Weights[i] == 0.0) {
				IDs[i] = boneID;
				Weights[i] = weight;
				return;
			}
		}
		ASSERT(false, "Trying to add more bones than there is space for");
	}
}