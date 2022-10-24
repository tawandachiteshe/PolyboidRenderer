#include "MeshLoader.h"

#include <spdlog/spdlog.h>
#include <stb/tiny_gltf.h>

#include "glad/glad.h"

namespace Polyboid
{
    std::vector<glm::vec2> MeshLoader::MakeVec2(const std::vector<float>& floats)
    {
        std::vector<glm::vec2> vecs;
        for (size_t i = 0; i < floats.size(); i)
        {
            vecs.emplace_back(glm::vec2(floats[i++], floats[i++]));
        }

        return vecs;
    }

    std::vector<glm::vec3> MeshLoader::MakeVec3(const std::vector<float>& floats)
    {
        std::vector<glm::vec3> vecs;
        for (size_t i = 0; i < floats.size(); i)
        {
            vecs.emplace_back(glm::vec3(floats[i++], floats[i++], floats[i++]));
        }

        return vecs;
    }

    std::vector<glm::vec4> MeshLoader::MakeVec4(const std::vector<float>& floats)
    {
        std::vector<glm::vec4> vecs;
        for (size_t i = 0; i < floats.size(); i)
        {
            vecs.emplace_back(glm::vec4(floats[i++], floats[i++], floats[i++], floats[i++]));
        }

        return vecs;
    }

    std::pair<ModelDataType, uint32_t> MeshLoader::QueryAccessorComponentType(const tinygltf::Accessor& accessor)
    {
        const auto componentType = accessor.componentType;
        ModelDataType modelDataType;
        int32_t componentByteSize;

        switch (componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            componentByteSize = 1;
            modelDataType = ModelDataType::uint8;
            break;
        case TINYGLTF_COMPONENT_TYPE_BYTE:
            componentByteSize = 1;
            modelDataType = ModelDataType::int8;
            break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            componentByteSize = 2;
            modelDataType = ModelDataType::uint16;
            break;
        case TINYGLTF_COMPONENT_TYPE_SHORT:
            componentByteSize = 2;
            modelDataType = ModelDataType::int16;
            break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            componentByteSize = 4;
            modelDataType = ModelDataType::uint32;
            break;
        case TINYGLTF_COMPONENT_TYPE_INT:
            componentByteSize = 4;
            modelDataType = ModelDataType::int32;
            break;
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            componentByteSize = 4;
            modelDataType = ModelDataType::f32;
            break;
        case TINYGLTF_COMPONENT_TYPE_DOUBLE:
            componentByteSize = 8;
            modelDataType = ModelDataType::f64;
            break;
        default:
            componentByteSize = 0;
        }

        return std::make_pair(modelDataType, componentByteSize);
    }

    MeshLoader::MeshLoader(const std::filesystem::path& path)
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF gltfCtx;
        std::string err;
        std::string warn;
        std::string pathString = path.string();

        bool isLoaded = false;

        const auto ext = path.extension().string();

        if(ext == ".glb")
        {
            isLoaded = gltfCtx.LoadBinaryFromFile(&model, &err, &warn, pathString);
        }
        else if (ext == ".gltf")
        {
            isLoaded = gltfCtx.LoadASCIIFromFile(&model, &err, &warn, pathString);
        }
       
      


        if (isLoaded)
        {
            for (auto& image : model.images)
            {
                PixelType pixel;
                if (image.component == 3)
                {
                    pixel = PixelType::RGB8_NORM;
                } else if (image.component == 4)
                {
                    pixel = PixelType::RGBA8_NORM;
                }
                
                m_Textures.push_back({ image.width, image.height, image.bits,image.image, pixel });
            }
            
            for (int i = 0; i < model.meshes[0].primitives.size(); ++i)
            {
                auto& primitive = model.meshes[0].primitives[i];
                const auto& positionAccIndex = primitive.attributes["POSITION"];
                const auto& normalAccIndex = primitive.attributes["NORMAL"];
                const auto& uvsACCIndex = primitive.attributes["TEXCOORD_0"];
                const auto& indices = primitive.indices;



                //Positions
                auto _positions = GetUsableDataFromBytes<float>(positionAccIndex, model);

                //Normals
                auto _normals = GetUsableDataFromBytes<float>(normalAccIndex, model);

                //UVS
                auto _UV = GetUsableDataFromBytes<float>(uvsACCIndex, model);


                //Indices
                const auto& accessor = model.accessors.at(indices);
                const auto [dataType, bytes] = QueryAccessorComponentType(accessor);

                m_IndexBufferByteOffset.push_back(accessor.byteOffset);

                std::vector<uint16_t> indices16;
                std::vector<uint32_t> indices32;

                if (dataType == ModelDataType::uint16)
                {
                    indices16 = GetUsableDataFromBytes<uint16_t>(indices, model);
                }
                else if (dataType == ModelDataType::uint32)
                {
                    indices32 = GetUsableDataFromBytes<uint32_t>(indices, model);
                }

               

                auto positions = MakeVec3(_positions);
                auto normals = MakeVec3(_normals);

                std::vector<glm::vec2> uvs;

                if (!_UV.empty() && uvsACCIndex != positionAccIndex)
                {
                     uvs = MakeVec2(_UV);

                }



                std::vector<Vertex> vertices;

                for (size_t i = 0; i < positions.size(); ++i)
                {
                	vertices.push_back({ positions[i], normals[i], uvs[i] });
                }

                m_Positions.emplace_back(_positions);
                m_Normals.emplace_back(_normals);
            	m_UV.emplace_back(_UV);
                

                m_Vertices.emplace_back(vertices);
                m_Indices16.push_back(indices16);
                m_Indices32.push_back(indices32);
            }
        }

        
    }

    Ref<MeshLoader> MeshLoader::LoadFile(const std::filesystem::path& path)
    {
        return std::make_shared<MeshLoader>(path);
    }
}
