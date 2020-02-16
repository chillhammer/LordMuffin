#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Graphics/Model/Model.h>
#include <Graphics/Model/ImportedSkinnedMesh.h>
#include <map>



namespace Skel
{

	/**
		Imports 3D skinned model based on file
		Specific to models with bones/animations
		Parser and Importer relying on library Assimp.
		Currently will account for multiple meshes, but only 1 diffuse texture.
	**/
	class ImportedSkinnedModel : public Model
	{
	public:
		ImportedSkinnedModel(char* path);
		void Draw(const ShaderPtr& shader, const Matrix4x4& model);
		void UpdateBoneTransforms(float runningTime);
		void UpdateBoneShader(ShaderPtr shader);
	private:
		struct BoneInfo
		{
			// Local space imported from file
			aiMatrix4x4 BoneOffset;
			// Calculated Output
			aiMatrix4x4 FinalTransformation;        

			BoneInfo() {}
		};
		void UpdateBoneNodeRecursive(float animationTime, const aiNode* node, const aiMatrix4x4& parentTransform);
		aiNodeAnim* GetNodeAnim(aiAnimation* anim, const std::string& nodeName) const;
		uint8 FindPosition(float animationTime, const aiNodeAnim* nodeAnim);
		uint8 FindRotation(float animationTime, const aiNodeAnim* nodeAnim);

		void CalcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* nodeAnim);
		void CalcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim);

		std::vector<ImportedSkinnedMesh> m_Meshes;
		std::vector<BoneInfo> m_BoneInfos;
		std::map<std::string, uint8> m_BoneIDMap;	// bone name to bone index
		std::map<std::string, aiAnimation*> m_AnimationMap;	// animation name to animation struct
		std::map<aiAnimation*, std::map<std::string, aiNodeAnim*>> m_AnimationNodeMap;	// animation struct to animation node
		const aiScene* m_Scene;
		Assimp::Importer m_Importer;
		aiMatrix4x4 m_GlobalInverseTransform;
		// These are a tracker that are used to see how much we have as we load
		uint8 m_NumBones = 0;
		int m_NumVertices = 0;

#pragma region Loading Meshes Externally
		std::string m_Directory;
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		ImportedSkinnedMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		Texture LoadMaterialTexture(aiMaterial* mat, aiTextureType type);
#pragma endregion
	};
}