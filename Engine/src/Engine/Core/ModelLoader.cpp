#include "ModelLoader.h"

#include <spdlog/spdlog.h>
#include <stb/tiny_gltf.h>

#include "glad/glad.h"

namespace Polyboid
{
    std::vector<glm::vec2> ModelLoader::MakeVec2(const std::vector<float>& floats)
    {
        std::vector<glm::vec2> vecs;
        for (size_t i = 0; i < floats.size(); i)
        {
            vecs.emplace_back(glm::vec2(floats[i++], floats[i++]));
        }

        return vecs;
    }

    std::vector<glm::vec3> ModelLoader::MakeVec3(const std::vector<float>& floats)
    {
        std::vector<glm::vec3> vecs;
        for (size_t i = 0; i < floats.size(); i)
        {
            vecs.emplace_back(glm::vec3(floats[i++], floats[i++], floats[i++]));
        }

        return vecs;
    }

    std::vector<glm::vec4> ModelLoader::MakeVec4(const std::vector<float>& floats)
    {
        std::vector<glm::vec4> vecs;
        for (size_t i = 0; i < floats.size(); i)
        {
            vecs.emplace_back(glm::vec4(floats[i++], floats[i++], floats[i++], floats[i++]));
        }

        return vecs;
    }

    std::pair<ModelDataType, uint32_t> ModelLoader::QueryAccessorComponentType(const tinygltf::Accessor& accessor)
    {
        const auto componentType = accessor.componentType;
        ModelDataType modelDataType;
        int32_t componentByteSize = 0;

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

    ModelLoader::ModelLoader(const std::filesystem::path& path)
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF gltfCtx;
        std::string err;
        std::string warn;
        std::string pathString = path.string();
        bool isLoaded = gltfCtx.LoadBinaryFromFile(&model, &err, &warn, pathString.c_str());


        if (isLoaded)
        {
            for (int i = 0; i < 2; ++i)
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
                if (dataType == ModelDataType::uint16)
                {
                    auto indices16 = GetUsableDataFromBytes<uint16_t>(indices, model);
                    m_Indices16.insert(m_Indices16.end(), indices16.begin(), indices16.end());
                }
                else if (dataType == ModelDataType::int32)
                {
                    auto indices32 = GetUsableDataFromBytes<uint32_t>(indices, model);
                    m_Indices32.insert(m_Indices32.end(), indices32.begin(), indices32.end());
                }

                auto positions = MakeVec3(_positions);
                auto normals = MakeVec3(_normals);
                auto uvs = MakeVec2(_UV);

                for (size_t i = 0; i < positions.size(); ++i)
                {
                    m_Vertices.push_back({positions[i], normals[i], uvs[i]});
                }

                m_Positions.insert(m_Positions.end(), _positions.begin(), _positions.end());
                m_Normals.insert(m_Normals.end(), _normals.begin(), _normals.end());
                m_UV.insert(m_UV.end(), _UV.begin(), _UV.end());
                
            }
            
        }
    }

    Ref<ModelLoader> ModelLoader::LoadFile(const std::filesystem::path& path)
    {
        return std::make_shared<ModelLoader>(path);
    }
}
