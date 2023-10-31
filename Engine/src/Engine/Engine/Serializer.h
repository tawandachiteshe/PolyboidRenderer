#pragma once
#include <spdlog/spdlog.h>

#include "Base.h"
#include "Debug/Timer.h"
#include "Utils/CubeMapFace.hpp"

namespace Polyboid
{
	enum class SerializerItemType
	{
		Int,

	};

	struct SerializerItem
	{
		uint32_t offset;
		uint32_t size;
	};

	struct SerializerSettings
	{
		uint32_t arrayCount;
		uint32_t valuesType;
	};

	class Serializer
	{
	private:
		std::vector<char> m_Bytes;
		SerializerSettings settings;
	public:
		Serializer() = default;
		Serializer(const std::vector<char>& bytes) : m_Bytes(bytes) {}

		std::vector<char>& GetBytes() { return  m_Bytes; }

		void Write(const std::string& filePath)
		{
			std::ofstream out(filePath, std::ios::binary);
			out.write(m_Bytes.data(), m_Bytes.size());
			out.close();
		}

		std::vector<char> Read(const std::string& filePath)
		{
			std::ifstream in(filePath, std::ios::binary);

			if (in)
			{
				std::vector<char> contents;
				in.seekg(0, std::ios::end);
				contents.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&contents[0], contents.size());
				in.close();

				m_Bytes = contents;

				return contents;
			}


			
		}

		template <typename T>
		T ReadValue(int32_t offset = 0)
		{
			
			const auto typeSize = static_cast<int>(sizeof(T));
			T value;
			char* bytes = (char*)&value;
			for (int i = 0; i < typeSize; ++i)
			{
				bytes[i] = m_Bytes[offset + i];
			}

			return value;
		}

		template <typename T>
		std::vector<T> ReadArray(int32_t count, int32_t offset = 0)
		{
			
			const auto typeSize = static_cast<int>(sizeof(T));

			std::vector<T> values;
			values.reserve((count * typeSize));

			for (int i = 0; i < (count * typeSize); i += typeSize)
			{

				values.emplace_back(ReadValue<T>(offset + i));

			}

			return values;
		}


		template<typename  T>
		void AddValue(T value)
		{
			auto typeSize = sizeof(value);

			char* bytes = (char*)&value;
			for (size_t i = 0; i < typeSize; ++i)
			{
				m_Bytes.push_back(bytes[i]);
			}


		}

		template<typename  T>
		void AddArray(std::vector<T> values)
		{
			auto typeSize = sizeof(T);
			for (auto& value : values)
			{
				char* bytes = (char*)&value;
				for (size_t i = 0; i < typeSize; ++i)
				{
					m_Bytes.push_back(bytes[i]);
				}
			}
		}

		template<>
		void AddValue(std::string value)
		{

			for (auto str : value)
			{
				m_Bytes.push_back(str);
			}

		}

	};

	class CubeMapSerializer
	{
	private:
		Serializer s;
		std::vector<std::vector<uint8_t>> m_Faces;

	public:
		struct Head
		{
			int32_t resolution;
			int32_t channels;
		};

		CubeMapSerializer()
		{
			m_Faces.reserve(6);
		}

		CubeMapSerializer(const Ref<CubeMapFace<uint8_t>>& face)
		{
			
			Head h{ face->GetResolution(), face->GetChannels() };
			s.AddValue<Head>(h);

			for (int i = 0; i != 6; ++i)
			{
				auto face_ = face->GetFaces()[i];
				std::vector<uint8_t> data;
				data.reserve(face->GetResolution() * face->GetResolution() * face->GetChannels());
				for (int i = 0; i < face->GetResolution() * face->GetResolution() * face->GetChannels(); ++i)
				{
					data.push_back(face_[i]);
				}
				s.AddArray<uint8_t>(data);
			}
		}

		Ref<CubeMapFace<uint8_t>> Read(const std::string& filePath)
		{
			m_Faces.reserve(6);
			
			s.Read(filePath);
			Head h = s.ReadValue<Head>();

			auto offset = (int32_t)sizeof(Head);
			

			for (int i = 0; i < (h.resolution * h.resolution * h.channels) * 6; i += (h.resolution * h.resolution * h.channels))
			{
				m_Faces.emplace_back(s.ReadArray<uint8_t>((h.resolution* h.resolution* h.channels), offset + i));
			}

			auto map = CreateRef<CubeMapFace<uint8_t>>(m_Faces, h.resolution, h.channels);

			return map;
		}

		void Write(const std::string& filePath)
		{
			s.Write(filePath);
		}

	};

	class MeshSerializer
	{

	private:
		Serializer s;
	public:
		MeshSerializer() = default;
		MeshSerializer(const std::vector<char>& bytes): s(bytes) {}


		struct Head
		{
			int32_t numofVerts;
			int32_t numofIndices;
			int32_t vertsOffset;
			int32_t indicesOffset;
		};



	};

}
