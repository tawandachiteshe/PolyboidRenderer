#include "boidpch.h"
#include "MeshImporter.h"

#include <spdlog/spdlog.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Engine/Renderer/VertexBufferArray.h"


namespace Polyboid
{
	RendererMeshData MeshImporter::Read(const std::filesystem::path& path)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate);

		if (!scene || !scene->HasMeshes())
		{
			spdlog::error("unable to load meshes");
			return {};
		}

		RendererMeshData data = {};


		//for now we get first mesh in a scene
		auto mesh = scene->mMeshes[0];

		if (mesh->HasPositions())
		{
			for (int i = 0; i < mesh->mNumVertices; ++i)
			{
				glm::vec3 vertex = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
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

	Ref<VertexBufferArray> MeshImporter::ReadForRendering(const std::filesystem::path& path)
	{
		auto meshData = Read(path);



		auto va = VertexBufferArray::MakeVertexBufferArray();
		va->Bind();

		auto ib = IndexBuffer::MakeIndexBuffer(meshData.Indices.data(), meshData.Indices.size());

		auto vb = VertexBuffer::MakeVertexBuffer(meshData.vertices.data(), meshData.vertices.size() * sizeof(float));
		vb->DescribeBuffer({
			{ BufferComponent::Float3, "aPosition" },
			{ BufferComponent::Float3, "aNormal" },
			{ BufferComponent::Float2, "aUV" }
		});

		va->AddVertexBuffer(vb);
		va->SetIndexBuffer(ib);

		return va;
	}
}
