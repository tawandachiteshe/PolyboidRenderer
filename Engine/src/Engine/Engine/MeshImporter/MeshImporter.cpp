#include "boidpch.h"
#include "MeshImporter.h"

#include <spdlog/spdlog.h>
#include <stb/stb_image.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Engine/Engine/AssetManager.h"
#include "Engine/Engine/Serializer.h"
#include "Engine/Engine/Utils/FileSystem.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/MaterialLibrary.h"
#include "Engine/Renderer/ShaderBufferStorage.h"
#include "Engine/Renderer/Texture2D.h"
#include "Engine/Renderer/VertexBufferArray.h"


namespace Polyboid
{
	RendererMeshData MeshImporter::GetMesh(const aiMesh* mesh, uint32_t meshIdx)
	{
		RendererMeshData data = {};


		//for now we get first mesh in a scene

		if (mesh->HasPositions())
		{
			for (int i = 0; i < mesh->mNumVertices; ++i)
			{
				glm::vec3 vertex = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
				data.vertex.positions.push_back(vertex);

				data.vertices.push_back(mesh->mVertices[i].x);
				data.vertices.push_back(mesh->mVertices[i].y);
				data.vertices.push_back(mesh->mVertices[i].z);


				data.HasColors = mesh->HasVertexColors(i);

				if (mesh->HasNormals())
				{
					data.HasNormals = true;
					data.vertex.normals.push_back({mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});
					data.vertices.push_back(mesh->mNormals[i].x);
					data.vertices.push_back(mesh->mNormals[i].y);
					data.vertices.push_back(mesh->mNormals[i].z);
				}


				//texture coord 0 for now
				if (mesh->mTextureCoords[0])
				{
					data.vertex.uvs.push_back({mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
					data.vertices.push_back(mesh->mTextureCoords[0][i].x);
					data.vertices.push_back(mesh->mTextureCoords[0][i].y);
				}

				if (mesh->HasTangentsAndBitangents())
				{
					data.vertices.push_back(mesh->mTangents[i].x);
					data.vertices.push_back(mesh->mTangents[i].y);
					data.vertices.push_back(mesh->mTangents[i].z);

					data.vertices.push_back(mesh->mBitangents[i].x);
					data.vertices.push_back(mesh->mBitangents[i].y);
					data.vertices.push_back(mesh->mBitangents[i].z);
				}

				data.vertices.push_back((float)meshIdx);

			}
		}


		if (mesh->HasFaces())
		{
			for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
			{
				const auto& face = mesh->mFaces[i];
				for (uint32_t j = 0; j < face.mNumIndices; ++j)
				{
					data.Indices.push_back(face.mIndices[j]);
				}
			}
		}

		return data;
	}

	static std::map<uint32_t, Ref<Material>> aiMatToEngine;
	static  std::map<int32_t, UUID> embeddedTextureMap;
	static  std::map<std::string, UUID> textureMap;
	static int32_t textureCount = 0;
	static int32_t textureMax = 27;

	static UUID LoadTexture(const std::string& path, const std::filesystem::path& meshPath)
	{
		UUID id = 0;

		auto mesh = meshPath.stem().string();

		if (textureMap.find(path) != textureMap.end())
		{
			id = textureMap[path];
		}
		else
		{
			
		}


		return id;
	}

	static UUID LoadEmbeddedTexture(const aiTexture* texture, int32_t textureIndex, const UUID& engineMaterialTexture)
	{
		UUID id = 0;
		if (embeddedTextureMap.find(textureIndex) != embeddedTextureMap.end())
		{
			id = embeddedTextureMap[textureIndex];
		}
		else
		{
			if (texture->mHeight == 0)
			{
				int32_t width = 0;
				int32_t height = 0;
				int32_t channels = 0;
				stbi_uc* data = stbi_load_from_memory((stbi_uc const*)texture->pcData, texture->mWidth, &width,
					&height, &channels, 0);

				AssetManager::LoadTexture(engineMaterialTexture,
					Texture::MakeTexture2D(width, height, channels, data));

				embeddedTextureMap[textureIndex] = engineMaterialTexture;
				id = engineMaterialTexture;

				stbi_image_free(data);
			}
		}

		return id;

	}

	static void LoadMaterial(const aiScene* scene, uint32_t matIdx, int32_t* textureIndices, const std::filesystem::path& meshPath)
	{
		auto* mat = scene->mMaterials[matIdx];

		aiColor3D diffuse(0.f, 0.f, 0.f);
		if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS)
		{
			aiMatToEngine[matIdx]->SetAlbedo({diffuse.r, diffuse.g, diffuse.b});
		}

		float roughness = 0.0f;
		if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
		{
			aiMatToEngine[matIdx]->SetRoughness(roughness);
		}

		float metallic = 0.0f;
		if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
		{
			aiMatToEngine[matIdx]->SetMetallic(metallic);
		}

		uint32_t diffuseTextures = mat->GetTextureCount(aiTextureType_DIFFUSE);
		uint32_t aoTextures = mat->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
		uint32_t metalnessTextures = mat->GetTextureCount(aiTextureType_METALNESS);
		uint32_t normalsTextures = mat->GetTextureCount(aiTextureType_NORMALS);

		aiString path;
		if (diffuseTextures)
		{
			if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), path) == AI_SUCCESS)
			{
				aiMatToEngine[matIdx]->mDiffuseTexture = UUID();

				auto [texture, textureIdx] = scene->GetEmbeddedTextureAndIndex(path.C_Str());
				if (textureIdx > -1)
				{
					textureIndices[0] = 0;
					aiMatToEngine[matIdx]->mDiffuseTexture = LoadEmbeddedTexture(texture, textureIdx, aiMatToEngine[matIdx]->mDiffuseTexture);
				}
				else
				{
					aiMatToEngine[matIdx]->mDiffuseTexture = LoadTexture(path.C_Str(), meshPath);
				}
			}

		}

		if (normalsTextures)
		{
			if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), path) == AI_SUCCESS)
			{
				aiMatToEngine[matIdx]->mNormalsTexture = UUID();
				auto [texture, textureIdx] = scene->GetEmbeddedTextureAndIndex(path.C_Str());
				if (textureIdx > -1)
				{
					textureIndices[1] = 1;
					aiMatToEngine[matIdx]->mNormalsTexture = LoadEmbeddedTexture(texture, textureIdx, aiMatToEngine[matIdx]->mNormalsTexture);
				}

			}
		}



		if (metalnessTextures)
		{
			if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_METALNESS, 0), path) == AI_SUCCESS)
			{
				aiMatToEngine[matIdx]->mMetallicTexture = UUID();
				auto [texture, textureIdx] = scene->GetEmbeddedTextureAndIndex(path.C_Str());
				if (textureIdx > -1)
				{
					textureIndices[2] = 2;
					aiMatToEngine[matIdx]->mMetallicTexture = LoadEmbeddedTexture(texture, textureIdx, aiMatToEngine[matIdx]->mMetallicTexture);
				}
			}
		}


		if (aoTextures)
		{
			if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT_OCCLUSION, 0), path) == AI_SUCCESS)
			{
				aiMatToEngine[matIdx]->mAOTexture = UUID();
				auto [texture, textureIdx] = scene->GetEmbeddedTextureAndIndex(path.C_Str());
				if (texture)
				{
					textureIndices[3] = 3;
					aiMatToEngine[matIdx]->mAOTexture = LoadEmbeddedTexture(texture, textureIdx, aiMatToEngine[matIdx]->mAOTexture);
				}

			}
		}

	}


	void MeshImporter::ProcessNode(aiNode* node, const aiScene* scene, MeshData& meshesData, const std::filesystem::path& path)
	{

		auto num = scene->mNumTextures;

		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			auto mesh = scene->mMeshes[node->mMeshes[i]];
			auto* mat = scene->mMaterials[mesh->mMaterialIndex];

			int32_t textureIdxs[4] = { -1, -1, -1, -1 };

			aiString matName;
			if (mat->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS)
			{
			}

			std::string name(matName.C_Str());
			if (aiMatToEngine.find(mesh->mMaterialIndex) != aiMatToEngine.end())
			{
				LoadMaterial(scene, mesh->mMaterialIndex, textureIdxs, path);
				aiMatToEngine[mesh->mMaterialIndex]->SetTextures(textureIdxs);
			}
			else
			{
				Ref<Material> engineMaterial = MaterialLibrary::CreateMaterial(UUID(), name);
				aiMatToEngine[mesh->mMaterialIndex] = engineMaterial;
				LoadMaterial(scene, mesh->mMaterialIndex, textureIdxs, path);
				engineMaterial->SetTextures(textureIdxs);



			}

			if (meshesData.find(aiMatToEngine[mesh->mMaterialIndex]) != meshesData.end())
			{
				meshesData[aiMatToEngine[mesh->mMaterialIndex]].push_back(GetMesh(mesh, node->mMeshes[i]));
			}
			else
			{
				meshesData[aiMatToEngine[mesh->mMaterialIndex]] = {};
				meshesData[aiMatToEngine[mesh->mMaterialIndex]].push_back(GetMesh(mesh, node->mMeshes[i]));
			}

		}


		for (int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene, meshesData, path);
		}
	}

	MeshData MeshImporter::Read(const std::filesystem::path& path)
	{
		//auto cachedMesh = FileSystem::GenCacheFilename(path.string(), ".polymesh");

		MeshData meshesData;

		Assimp::Importer importer;

		unsigned int importFlags =
			aiProcess_GenSmoothNormals
			| aiProcess_JoinIdenticalVertices
			| aiProcess_ImproveCacheLocality
			| aiProcess_LimitBoneWeights
			| aiProcess_RemoveRedundantMaterials
			| aiProcess_SplitLargeMeshes
			| aiProcess_Triangulate
			| aiProcess_GenUVCoords
			| aiProcess_SortByPType
			| aiProcess_Triangulate
			| aiProcess_CalcTangentSpace
			| aiProcess_EmbedTextures;

		const aiScene* scene = importer.ReadFile(path.string(), importFlags);

		if (!scene || !scene->HasMeshes())
		{
			spdlog::error("unable to load meshes");
			return {};
		}


		ProcessNode(scene->mRootNode, scene, meshesData, path);
		aiMatToEngine.clear();
		embeddedTextureMap.clear();
		textureCount = 0;

		return meshesData;
	}

	static  void CombineBuffers(const MeshData& _meshData, MeshDataRenderer& _vertexBuffers)
	{
		std::vector<MaterialData> materialsData;
		std::vector<Ref<Material>> _materials;

		constexpr uint32_t MAX_MATERIALS = 5000;

		//Combine Trial


		Ref<ShaderBufferStorage> materialStorage = ShaderBufferStorage::Make(sizeof(MaterialData) * MAX_MATERIALS);

		uint32_t matOffset = 0;

		for (auto& meshes : _meshData)
		{

			auto& [material, meshData] = meshes;

			uint32_t offset = 0;
			uint32_t meshCount = 0;

			std::vector<float> hugeVerts;
			std::vector<uint32_t> hugeIdx;

			for (auto& mesh : meshData)
			{

				hugeVerts.insert(hugeVerts.end(), mesh.vertices.begin(), mesh.vertices.end());

				for (auto idx : mesh.Indices)
				{
					hugeIdx.push_back(idx + offset);
				}

				offset += (mesh).vertex.positions.size();


			}

			auto indices = ShaderBufferStorage::Make(hugeIdx.data(), hugeIdx.size() * sizeof(uint32_t));

			Ref<ShaderBufferStorage> verts;
			auto va = VertexBufferArray::MakeVertexBufferArray(indices, hugeIdx.size());
			verts = ShaderBufferStorage::Make(hugeVerts.data(), hugeVerts.size() * sizeof(float));

			va->SetShaderBufferStorage(verts);

			_vertexBuffers[material] = va;

		}


	}


	

	MeshDataRenderer MeshImporter::ReadForRendering(const std::filesystem::path& path)
	{
		auto _meshData = Read(path);

		//std::vector<Ref<VertexBufferArray>> vas(2);
		MeshDataRenderer _vertexBuffers;

		//Combine Trial
		CombineBuffers(_meshData, _vertexBuffers);

		return _vertexBuffers;
	}
}
