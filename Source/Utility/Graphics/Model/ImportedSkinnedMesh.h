#pragma once
#include "Mesh.h"
namespace Skel {
	#define NUM_BONES_PER_VERTEX 4
	struct SkinnedVertex
	{
		Vector3 Position;
		Vector3 Normal;
		Vector2 TexCoords;
		unsigned int IDs[NUM_BONES_PER_VERTEX];
		float Weights[NUM_BONES_PER_VERTEX];
		void AddBoneData(int boneID, float weight);
		SkinnedVertex() {
			memset(Weights, 0, sizeof(Weights[0]) * NUM_BONES_PER_VERTEX);
		}
	};
	/**
		Specific mesh designed for import.
	**/
	class ImportedSkinnedMesh :
		public Mesh
	{
	public:
		ImportedSkinnedMesh(std::vector<SkinnedVertex> vertexBuffer, std::vector<unsigned int> indexBuffer,
			Texture texture);
		ImportedSkinnedMesh(std::vector<SkinnedVertex> vertexBuffer, std::vector<unsigned int> indexBuffer,
			Vector3 color);
		ImportedSkinnedMesh(const ImportedSkinnedMesh& other);
		virtual ~ImportedSkinnedMesh();
	private:
		std::vector<SkinnedVertex> m_Vertices;
		std::vector<unsigned int> m_Indicies;

		virtual VertexBufferLayout GetBufferLayout() override;
	};
}