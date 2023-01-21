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

	static size_t countVertices(MeshDataIdxArray& miArray, const aiScene* scene, const aiNode* node) {
		if (nullptr == scene) {
			return 0;
		}

		size_t numVertices = 0;
		for (unsigned int i = 0; i < miArray.size(); ++i) {
			
			numVertices += static_cast<size_t>(scene->mMeshes[miArray[i]]->mNumVertices);
		}

		return numVertices;
	}

	RendererMeshData MeshImporter::GetMesh(const aiMesh* mesh)
	{
		RendererMeshData data = {};


		//for now we get first mesh in a scene

		if (mesh->HasPositions())
		{
			for (int i = 0; i < mesh->mNumVertices; ++i)
			{
				glm::vec3 vertex = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
				data.vertices.push_back(mesh->mVertices[i].x);
				data.vertices.push_back(mesh->mVertices[i].y);
				data.vertices.push_back(mesh->mVertices[i].z);


				data.HasColors = mesh->HasVertexColors(i);

				// if (mesh->HasVertexColors(i))
				// {
				// 	data.vertex.colors.push_back({
				// 		mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a
				// 	});
				//
				// 	data.vertices.push_back(mesh->mColors[0][i].r);
				// 	data.vertices.push_back(mesh->mColors[0][i].g);
				// 	data.vertices.push_back(mesh->mColors[0][i].b);
				// 	data.vertices.push_back(mesh->mColors[0][i].a);
				//
				// }

				if (mesh->HasNormals())
				{
					data.HasNormals = true;
					data.vertex.normals.push_back({ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
					data.vertices.push_back(mesh->mNormals[i].x);
					data.vertices.push_back(mesh->mNormals[i].y);
					data.vertices.push_back(mesh->mNormals[i].z);

				}

		

				//texture coord 0 for now
				if (mesh->mTextureCoords[0])
				{
					data.vertex.uvs.push_back({ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
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

				data.vertex.positions.push_back(vertex);
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


	void MeshImporter::ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::pair<RendererMeshData, Ref<Material>>>& meshesData)
	{
	

		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			auto mesh = scene->mMeshes[node->mMeshes[i]];
			auto* mat= scene->mMaterials[mesh->mMaterialIndex];

			aiString matName;
			if (mat->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS)
			{
			}

			std::string name(matName.C_Str());
			Ref<Material> engineMaterial = MaterialLibrary::CreateMaterial(UUID(), name);

			

			aiColor3D diffuse(0.f, 0.f, 0.f);
			if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS)
			{
				engineMaterial->SetAlbedo({ diffuse.r, diffuse.g, diffuse.b });
			}

			float roughness = 0.0f;
			if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
			{
				engineMaterial->SetRoughness(roughness);
			}

			float metallic = 0.0f;
			if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
			{
				engineMaterial->SetMetallic(metallic);
			}

			int diffuseTextures = mat->GetTextureCount(aiTextureType_DIFFUSE);
			int aoTextures = mat->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
			int metalnessTextures = mat->GetTextureCount(aiTextureType_METALNESS);
			int normalsTextures = mat->GetTextureCount(aiTextureType_NORMALS);
			int roughnessTextures = mat->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
			
			aiString path;
			if (diffuseTextures)
			{
				
				if(mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), path) == AI_SUCCESS)
				{
					engineMaterial->mDiffuseTexture = UUID();
					auto texture = scene->GetEmbeddedTexture(path.C_Str());
					if (texture->mHeight == 0)
					{
						int32_t width = 0;
						int32_t height = 0;
						int32_t channels = 0;
						stbi_uc* data = stbi_load_from_memory((stbi_uc const*) texture->pcData, texture->mWidth, &width, &height, &channels, 0);


						AssetManager::LoadTexture(engineMaterial->mDiffuseTexture, Texture::MakeTexture2D(width, height, channels, data));

						if (data != nullptr)
						{
							stbi_image_free(data);
						}
					}

					
				}
				
			}

			if (aoTextures)
			{

				if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT_OCCLUSION, 0), path) == AI_SUCCESS)
				{
					engineMaterial->mAOTexture = UUID();
					auto texture = scene->GetEmbeddedTexture(path.C_Str());
					if (texture->mHeight == 0)
					{
						int32_t width = 0;
						int32_t height = 0;
						int32_t channels = 0;
						stbi_uc* data = stbi_load_from_memory((stbi_uc const*)texture->pcData, texture->mWidth, &width, &height, &channels, 0);

						AssetManager::LoadTexture(engineMaterial->mAOTexture, Texture::MakeTexture2D(width, height, channels, data));

						if (data != nullptr)
						{
							stbi_image_free(data);
						}
					}

				}


			}

			if (metalnessTextures)
			{
				if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_METALNESS, 0), path) == AI_SUCCESS)
				{
					engineMaterial->mMetallicTexture = UUID();
					auto texture = scene->GetEmbeddedTexture(path.C_Str());
					if (texture->mHeight == 0)
					{
						int32_t width = 0;
						int32_t height = 0;
						int32_t channels = 0;
						stbi_uc* data = stbi_load_from_memory((stbi_uc const*)texture->pcData, texture->mWidth, &width, &height, &channels, 0);

						AssetManager::LoadTexture(engineMaterial->mMetallicTexture, Texture::MakeTexture2D(width, height, channels, data));

						if (data != nullptr)
						{
							stbi_image_free(data);
						}
					}

				}


			}

			if (normalsTextures)
			{
				if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), path) == AI_SUCCESS)
				{
					engineMaterial->mNormalsTexture = UUID();
					auto texture = scene->GetEmbeddedTexture(path.C_Str());
					if (texture->mHeight == 0)
					{
						int32_t width = 0;
						int32_t height = 0;
						int32_t channels = 0;
						stbi_uc* data = stbi_load_from_memory((stbi_uc const*)texture->pcData, texture->mWidth, &width, &height, &channels, 0);

						AssetManager::LoadTexture(engineMaterial->mNormalsTexture, Texture::MakeTexture2D(width, height, channels, data));

						if (data != nullptr)
						{
							stbi_image_free(data);
						}
					}

					
				}


			}


			if (roughnessTextures)
			{
				if (mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE_ROUGHNESS, 0), path) == AI_SUCCESS)
				{
					engineMaterial->mRoughnessTexture = UUID();
					auto texture = scene->GetEmbeddedTexture(path.C_Str());
					if (texture->mHeight == 0)
					{
						int32_t width = 0;
						int32_t height = 0;
						int32_t channels = 0;
						stbi_uc* data = stbi_load_from_memory((stbi_uc const*)texture->pcData, texture->mWidth, &width, &height, &channels, 0);

						AssetManager::LoadTexture(engineMaterial->mRoughnessTexture, Texture::MakeTexture2D(width, height, channels, data));

						if (data != nullptr)
						{
							stbi_image_free(data);
						}
					}

				
				}

			}

			meshesData.emplace_back(std::make_pair(GetMesh(mesh), engineMaterial));
		}

	
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene, meshesData);
		}


	}

	std::vector<std::pair<RendererMeshData, Ref<Material>>> MeshImporter::Read(const std::filesystem::path& path)
	{
		//auto cachedMesh = FileSystem::GenCacheFilename(path.string(), ".polymesh");

		std::vector<std::pair<RendererMeshData, Ref<Material>>> meshesData;
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
		;

		const aiScene* scene = importer.ReadFile(path.string(), importFlags);

		if (!scene || !scene->HasMeshes())
		{
			spdlog::error("unable to load meshes");
			return {};
		}

		ProcessNode(scene->mRootNode, scene, meshesData);


		return meshesData;
	}

	std::vector<std::pair<Ref<VertexBufferArray>, Ref<Material>>> MeshImporter::ReadForRendering(const std::filesystem::path & path)
	{
		auto _meshData = Read(path);

		//std::vector<Ref<VertexBufferArray>> vas(2);
		std::vector<std::pair<Ref<VertexBufferArray>, Ref<Material>>> _vertexBuffers;
		

		for (auto& meshData : _meshData)
		{
			auto indices = ShaderBufferStorage::Make(meshData.first.Indices.data(), meshData.first.Indices.size() * sizeof(uint32_t));
		
			auto va = VertexBufferArray::MakeVertexBufferArray(indices, meshData.first.Indices.size());
		
			auto verts = ShaderBufferStorage::Make(meshData.first.vertices.data(), meshData.first.vertices.size() * sizeof(float));
			va->SetShaderBufferStorage(verts);
		
			auto matVert = std::make_pair(va, meshData.second);
		
			_vertexBuffers.push_back(matVert);
		}

		 // for (auto& meshData : _meshData)
		 // {
		 //
		 // 	auto va = VertexBufferArray::MakeVertexBufferArray();
		 // 	va->Bind();
		 //
		 // 	auto ib = IndexBuffer::MakeIndexBuffer(meshData.first.Indices.data(), meshData.first.Indices.size());
		 //
		 // 	auto vb = VertexBuffer::MakeVertexBuffer(meshData.first.vertices.data(), meshData.first.vertices.size() * sizeof(float));
		 // 	vb->DescribeBuffer({
		 // 		{ BufferComponent::Float3, "aPosition" },
		 // 		{ BufferComponent::Float3, "aNormal" },
		 // 		{ BufferComponent::Float2, "aUV" }
		 // 		});
		 //
		 // 	va->AddVertexBuffer(vb);
		 // 	va->SetIndexBuffer(ib);
		 //
			// auto matVert = std::make_pair(va, meshData.second);
		 // 	_vertexBuffers.push_back(matVert);
		 // }





		return _vertexBuffers;
	}
}
