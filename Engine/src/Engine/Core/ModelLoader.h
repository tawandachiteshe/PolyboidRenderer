#pragma once

#include <filesystem>
#include <spdlog/spdlog.h>
#include <stb/tiny_gltf.h>

#include "Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Polyboid
{
    struct Vertex
    {
        glm::vec3 positions;
        glm::vec3 normals;
        glm::vec2 UVs;
    };
    
    enum class ModelDataType
    {
        int8,
        uint8,
        int16,
        uint16,
        int32,
        uint32,
        f32,
        f64
    };
    
    
    class ModelLoader
    {
    private:

        std::vector<Vertex> m_Vertices;
        std::vector<float> m_Positions;
        std::vector<float> m_Normals;
        std::vector<float> m_UV;
        std::vector<float> m_FloatVertices;
        std::vector<uint32_t> m_Indices32;
        std::vector<uint16_t> m_Indices16;

        static  std::vector<glm::vec2> MakeVec2(const std::vector<float>& floats);
        static  std::vector<glm::vec3> MakeVec3(const std::vector<float>& floats);
        static  std::vector<glm::vec4> MakeVec4(const std::vector<float>& floats);

        template <typename T>
        std::vector<T> GetUsableDataFromBytes(uint32_t accessorIndex, tinygltf::Model& model);

        std::pair<ModelDataType, uint32_t> QueryAccessorComponentType(const tinygltf::Accessor& accessor);

    public:
        ModelLoader(const std::filesystem::path& path);

        static Ref<ModelLoader> LoadFile(const std::filesystem::path& path);
        std::vector<float>& GetPositions() { return m_Positions; }
        std::vector<float>& GetAllFloatVertices() { return m_FloatVertices; }
        std::vector<float>& GetNormals() { return m_Normals; }
        std::vector<Vertex>& GetVertices() { return m_Vertices; }
        std::vector<float>& GetUVs() { return m_UV; }
        std::vector<uint32_t>& GetIndices32() { return m_Indices32; }
        std::vector<uint16_t>& GetIndices16() { return m_Indices16; }
    };


    template <typename T>
   std::vector<T>  ModelLoader::GetUsableDataFromBytes(uint32_t accessorIndex, tinygltf::Model& model)
    {
        const auto& accessor = model.accessors.at(accessorIndex);
        const auto bufferIndex = accessor.bufferView;
        const auto count = accessor.count;
        const auto accByteOffset = accessor.byteOffset;
        const auto type = accessor.type;

        const auto& bufferView = model.bufferViews.at(bufferIndex);
        const auto byteOffset = bufferView.byteOffset;

        const auto& data = model.buffers.at(0).data.data();

        uint32_t numPerVert = 0;

        switch (type)
        {
        case TINYGLTF_TYPE_SCALAR:
            numPerVert = 1;
            break;
        case TINYGLTF_TYPE_VEC2:
            numPerVert = 2;
            break;
        case TINYGLTF_TYPE_VEC3:
            numPerVert = 3;
            break;
        case TINYGLTF_TYPE_VEC4:
            numPerVert = 4;
            break;
        default:
            numPerVert = 0;
        }

        std::vector<T> values;

        auto [modelDataType, componentByteSize] = QueryAccessorComponentType(accessor);
        


        const auto startOfData = byteOffset + accByteOffset;
        const auto lengthOfData = count * componentByteSize * numPerVert;

        if (accessorIndex == 3)
        {
            spdlog::warn("last Index");
        }

        for (size_t i = startOfData; i < startOfData + lengthOfData; i)
        {
            uint8_t* bytes = nullptr;

            switch (componentByteSize)
            {
            case 1:
                bytes = new uint8_t[1];
                bytes[0] = data[i++];
                break;
            case 2:
                bytes = new uint8_t[2];
                bytes[0] = data[i++];
                bytes[1] = data[i++];
                break;
            case 4:
                bytes = new uint8_t[4];
                bytes[0] = data[i++];
                bytes[1] = data[i++];
                bytes[2] = data[i++];
                bytes[3] = data[i++];
                break;
            case 8:
                bytes = new uint8_t[8];
                bytes[0] = data[i++];
                bytes[1] = data[i++];
                bytes[2] = data[i++];
                bytes[3] = data[i++];
                bytes[4] = data[i++];
                bytes[5] = data[i++];
                bytes[6] = data[i++];
                bytes[7] = data[i++];
                break;
            default:
                bytes = nullptr;
            }

            if (bytes)
            {
                T value = 0;
                std::memcpy(&value, bytes, sizeof(value));
                values.push_back(value);
                delete[] bytes;
            }
        }

        return values;
    }
}
