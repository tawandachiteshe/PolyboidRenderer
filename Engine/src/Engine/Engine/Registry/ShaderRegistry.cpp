#include "boidpch.h"
#include "ShaderRegistry.h"

#include <execution>
#include <spdlog/spdlog.h>
#include <taskflow/taskflow.hpp>

#include "Engine/Engine/Base.h"
#include "Engine/Engine/Debug/Timer.h"
#include "Engine/Renderer/Shader.h"


namespace Polyboid
{
	class Shader;

	std::unordered_map<std::string, ShaderBinaryAndInfo> ShaderRegistry::s_Registry;
	std::unordered_map<std::string, Ref<Shader>> ShaderRegistry::s_RegistryShader;

	static const std::filesystem::path cacheFolder = "Resources/.Cache";
	static const std::filesystem::path shaderPath = "Resources/Shaders";
	static const std::filesystem::path shaderCache = "Resources/.Cache/Shaders";


	void ShaderRegistry::Init()
	{
		if (!std::filesystem::exists(cacheFolder))
		{
			std::filesystem::create_directory(cacheFolder);
		}


		if (!std::filesystem::exists(shaderCache))
		{
			s_Registry = ShaderCompiler::CompileShadersFromPath(shaderPath.string());
			std::filesystem::create_directory(shaderCache);
		}


		if (!s_Registry.empty())
		{
			ShaderCompiler::Dump(s_Registry, shaderCache);
		}
		else
		{
			s_Registry = ShaderCompiler::LoadFromDump(shaderCache);
		}



	}

	bool ShaderRegistry::Exist(const std::string& path)
	{
		return s_RegistryShader.find(path) != s_RegistryShader.end();
	}

	Ref<Shader> ShaderRegistry::Load(const std::string& path)
	{
		if (!Exist(path))
		{
			auto info = s_Registry[path];
			auto shader = Shader::Create(info);
			s_RegistryShader[path] = shader;

			return shader;
		}

		return s_RegistryShader[path];
	}

	Ref<Shader> ShaderRegistry::Create(const std::string& path)
	{
		auto info = ShaderCompiler::Compile(path, "");
		s_Registry[path] = info;

		return Shader::Create(info);
	}
}
