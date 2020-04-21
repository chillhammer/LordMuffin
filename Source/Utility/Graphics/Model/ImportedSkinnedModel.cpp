#include <skelpch.h>
#include <Game/GameManager.h>
#include <Resources/ResourceManager.h>
#include "ImportedSkinnedModel.h"
namespace Skel
{
	// Imports model through given file path
	ImportedSkinnedModel::ImportedSkinnedModel(char* path) : m_CurrentAnimation(nullptr)
	{
		LoadModel(path);
		m_CurrentAnimation = m_Scene->mAnimations[0];
		LOG("Imported Model: {0}", path);
	}

	// Loops Through All Meshes, Then Draws
	void ImportedSkinnedModel::Draw(const ShaderPtr& shader, const Matrix4x4& model)
	{
		UpdateBoneTransforms(Game.RunningTime());
		UpdateBoneShader(Resources.GetShader("SkinnedModel"));

		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].Draw(shader, model);
	}

	// Runs the animation and updates bone transforms
	void ImportedSkinnedModel::UpdateBoneTransforms(float runningTime)
	{
		ASSERT(m_Scene, "Scene must exist");
		aiMatrix4x4 identity;

		if (m_Scene->mNumAnimations == 0) {
			LOG_WARN("Skinned Model has no animations");
			return;
		}

		aiAnimation* animation = m_CurrentAnimation;
		ASSERT(animation != nullptr, "Animation does not exist");
		float ticksPerSecond = (float)(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f);
		float timeInTicks = runningTime * ticksPerSecond;
		float animationDuration = (float)animation->mDuration;
		float animationTime = std::fmod(timeInTicks, animationDuration);

		//LOG("Name: {4} | TPS: {0} | TimeInTicks: {1} | Duration: {2}, Time {3}", ticksPerSecond, timeInTicks, animationDuration, animationTime, animation->mName.C_Str());


		UpdateBoneNodeRecursive(animationTime, m_Scene->mRootNode, identity);

	}

	void ImportedSkinnedModel::UpdateBoneShader(ShaderPtr shader)
	{
		uint16 bonesSize = m_BoneInfos.size();
		for (uint16 i = 0; i < bonesSize; ++i) {
			char uniformName[16];
			memset(uniformName, 0, sizeof(uniformName));

			snprintf(uniformName, sizeof(uniformName), "u_Bones[%d]", i);
			std::stringstream ss;
			ss.clear();
			ss << "u_Bones[" << i << "]";
			aiMatrix4x4& boneTrans = m_BoneInfos[i].FinalTransformation;
			boneTrans = boneTrans.Transpose();
			Matrix4x4 matrix(boneTrans.a1, boneTrans.a2, boneTrans.a3, boneTrans.a4,
				boneTrans.b1, boneTrans.b2, boneTrans.b3, boneTrans.b4,
				boneTrans.c1, boneTrans.c2, boneTrans.c3, boneTrans.c4,
				boneTrans.d1, boneTrans.d2, boneTrans.d3, boneTrans.d4);
			//matrix = Matrix4x4::transpose_type(matrix);
			//matrix = Matrix4x4(1.0f);

			shader->Bind();
			shader->SetUniformMat4f(ss.str(), matrix);
		}
	}

	// Updates bone transforms by recursively going through hierachy
	// Helper function
	void ImportedSkinnedModel::UpdateBoneNodeRecursive(float animationTime, const aiNode* node, const aiMatrix4x4& parentTransform)
	{
		std::string nodeName(node->mName.data);
		
		aiAnimation* animation = m_CurrentAnimation;

		aiMatrix4x4 nodeTransformation(node->mTransformation);

		const aiNodeAnim* nodeAnim = GetNodeAnim(animation, nodeName);

		if (nodeAnim) {
			// scaling

			// rotation
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, animationTime, nodeAnim);
			aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

			// position
			aiVector3D translation;
			CalcInterpolatedPosition(translation, animationTime, nodeAnim);
			aiMatrix4x4 translationM;
			aiMatrix4x4::Translation(translation, translationM);

			// Combine the above transformations
			nodeTransformation = translationM * RotationM; // *ScalingM;
		}

		aiMatrix4x4 GlobalTransformation = parentTransform * nodeTransformation;

		// Update bone final transformation!
		if (m_BoneIDMap.find(nodeName) != m_BoneIDMap.end()) {
			uint8 BoneIndex = m_BoneIDMap[nodeName];
			m_BoneInfos[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfos[BoneIndex].BoneOffset;


			//aiVector3D translation(1.0f + animationTime, 0.0f, 0.0f);
			//aiMatrix4x4::Translation(translation, m_BoneInfos[BoneIndex].FinalTransformation);
			if (nodeAnim) {
				aiQuaternion RotationQ;
				//aiQuaternion rot(aiVector3D(1.0f, 0.0f, 0.0f), animationTime * 100.0f);
				CalcInterpolatedRotation(RotationQ, animationTime, nodeAnim);
				
				aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());
				//aiMatrix4x4 RotationM = aiMatrix4x4(rot.GetMatrix());
				//m_BoneInfos[BoneIndex].FinalTransformation = RotationM;
			}
		}

		for (uint8 i = 0; i < node->mNumChildren; i++) {
			UpdateBoneNodeRecursive(animationTime, node->mChildren[i], GlobalTransformation);
		}
	}

	aiNodeAnim* ImportedSkinnedModel::GetNodeAnim(aiAnimation* anim, const std::string& nodeName) const
	{
		// Looks for animation, gives map of [node -> nodeAnim]
		auto& map = m_AnimationNodeMap.find(anim)->second;
		auto& nodeAnim = map.find(nodeName);
		if (nodeAnim == map.end())
			return nullptr;
		return nodeAnim->second;
	}
	uint8 ImportedSkinnedModel::FindPosition(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (uint8 i = 0; i < nodeAnim->mNumPositionKeys - 1; i++) {
			if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}

		ASSERT(false, "Must find a position");

		return 0;
	}


	uint8 ImportedSkinnedModel::FindRotation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		ASSERT(nodeAnim->mNumRotationKeys > 0, "Must have rotation keys");

		for (uint8 i = 0; i < nodeAnim->mNumRotationKeys - 1; i++) {
			//LOG((float)nodeAnim->mRotationKeys[i+1].mTime);
			if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}

		ASSERT(false, "Must find a rotation");

		return 0;
	}

	uint8 ImportedSkinnedModel::GetAnimationIndex(const std::string& animName) const
	{
		auto animIt = m_AnimationMap.find(animName);
		ASSERT(animIt != m_AnimationMap.end(), "Animation does not exist");
		return animIt->second.second;
	}

	void ImportedSkinnedModel::CalcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumPositionKeys == 1) {
			out = nodeAnim->mPositionKeys[0].mValue;
			return;
		}

		uint8 PositionIndex = FindPosition(animationTime, nodeAnim);
		uint8 NextPositionIndex = (PositionIndex + 1);
		ASSERT(NextPositionIndex < nodeAnim->mNumPositionKeys, "Next position index not in range");
		float DeltaTime = (float)(nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (animationTime - (float)nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		//ASSERT(Factor >= 0.0f && Factor <= 1.0f, "Position Interpolatio factor not in range");
		const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		out = Start + Factor * Delta;
	}


	void ImportedSkinnedModel::CalcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* nodeAnim)
	{
		// we need at least two values to interpolate...
		if (nodeAnim->mNumRotationKeys == 1) {
			out = nodeAnim->mRotationKeys[0].mValue;
			return;
		}

		uint8 rotationIndex = FindRotation(animationTime, nodeAnim);
		uint8 nextRotationIndex = (rotationIndex + 1);
		ASSERT(nextRotationIndex < nodeAnim->mNumRotationKeys, "Next rotation index not in range");
		float DeltaTime = (float)(nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime);
		float Factor = (animationTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / DeltaTime;
		//ASSERT(Factor >= 0.0f && Factor <= 1.0f, "Factor from rotation key is not within range");
		const aiQuaternion& StartRotationQ = nodeAnim->mRotationKeys[rotationIndex].mValue;
		const aiQuaternion& EndRotationQ = nodeAnim->mRotationKeys[nextRotationIndex].mValue;
		aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
		out = out.Normalize();
	}



	// Base function to load in model
	void ImportedSkinnedModel::LoadModel(std::string path)
	{
		
		m_Scene = m_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

		if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode)
		{
			LOG_ERROR("ASSIMP::{0}", m_Importer.GetErrorString());
			return;
		}
		m_GlobalInverseTransform = m_Scene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();

		// process animations
		m_AnimationMap.clear();
		m_AnimationNodeMap.clear();
		for (int i = 0; i < m_Scene->mNumAnimations; ++i) {
			aiAnimation* animation = m_Scene->mAnimations[i];
			std::string animName = animation->mName.C_Str();
			LOG("Loading animation[{0}]: {1}", i, animName);
			m_AnimationMap[animName] = std::pair<aiAnimation*, uint8>(animation, i);
			std::map<std::string, aiNodeAnim*> nodeAnimMap;
			for (int o = 0; o < animation->mNumChannels; ++o) {
				std::string animNodeName = animation->mChannels[o]->mNodeName.C_Str();
				nodeAnimMap[animNodeName] = animation->mChannels[o];
			}
			m_AnimationNodeMap[animation] = nodeAnimMap;
		}

		m_Directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(m_Scene->mRootNode, m_Scene);
	}

	// Finds ands processes all meshes
	void ImportedSkinnedModel::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	ImportedSkinnedMesh ImportedSkinnedModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Parameters for ImportedMesh
		std::vector<SkinnedVertex> vertices;
		std::vector<unsigned int> indices;

		// Get Vertex Data
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			SkinnedVertex vertex;
			vertex.Position =
			{
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};
			vertex.Normal =
			{
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
			};
			// Bones
			for (int o = 0; o < mesh->mNumBones; ++o) {
				uint8 boneIndex = 0; 
				aiBone* bone = mesh->mBones[o];
				std::string boneName(bone->mName.data);

				// Find bone index/id
				if (m_BoneIDMap.find(boneName) == m_BoneIDMap.end()) {
					boneIndex = m_NumBones;
					m_NumBones++;
					m_BoneInfos.emplace_back();
				}
				else {
					boneIndex = m_BoneIDMap[boneName];
				}

				m_BoneIDMap[boneName] = boneIndex;
				m_BoneInfos[boneIndex].BoneOffset = bone->mOffsetMatrix;

				// Add each bone weight
				for (int j = 0; j < bone->mNumWeights; j++) {
					//uint8 vertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
					aiVertexWeight weightInfo = bone->mWeights[j];
					// Note: not completely sure 'i' is the same as vertex ID. Maybe use m_NumVertices
					if (weightInfo.mVertexId == i) {
						// Add bone data
						vertex.AddBoneData(boneIndex, weightInfo.mWeight);
					}
				}
			}
			
			// Get TextureCoordinates if Exists
			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoords =
				{
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}
			else
				vertex.TexCoords = { 0.f, 0.f };
			vertices.push_back(vertex);
		}
		// Update Total Vertices Count
		m_NumVertices += mesh->mNumVertices;

		// Get Index Data
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Get Texture or Color based on Material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		Texture diffuseTexture = LoadMaterialTexture(material, aiTextureType_DIFFUSE);
		if (diffuseTexture.GetFilePath() == "")
		{
			aiColor3D color;
			bool getColor = AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			ASSERT(getColor, "Failed to find color or texture");
			aiString name;
			bool getName = AI_SUCCESS == material->Get(AI_MATKEY_NAME, name);
			ASSERT(getName, "Failed to get color name");
			LOG("Sucessfully loaded color material: {0}", name.C_Str());
			return ImportedSkinnedMesh(vertices, indices, Vector3(color.r, color.g, color.b));
		}
		return ImportedSkinnedMesh(vertices, indices, diffuseTexture);
	}

	// Creates texture based on file path
	Texture ImportedSkinnedModel::LoadMaterialTexture(aiMaterial* mat, aiTextureType type)
	{
		aiString str;
		if (AI_SUCCESS == mat->GetTexture(type, 0, &str))
		{
			LOG("Sucessfully loaded texture material: {0}", m_Directory + '/' + str.C_Str());
			return Texture(m_Directory + '/' + str.C_Str());
		}
		return Texture();
	}
}