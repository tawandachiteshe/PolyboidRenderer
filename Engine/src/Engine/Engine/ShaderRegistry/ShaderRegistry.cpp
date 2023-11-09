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

	std::unordered_map<std::string, ShaderBinaryAndReflectionInfo> ShaderRegistry::s_Registry;
	std::unordered_map<std::string, Ref<Shader>> ShaderRegistry::s_RegistryShader;

	static const std::filesystem::path cacheFolder = "Resources/.Cache";
	static const std::filesystem::path shaderPath = "Resources/Shaders";
	static const std::filesystem::path shaderCache = "Resources/.Cache/Shaders";


	void ShaderRegistry::Init(RenderAPI* context)
	{

		ShaderCompiler::Init(context, shaderPath.generic_string());


		s_Registry = ShaderCompiler::CompileShadersFromPath(shaderPath.string());

		// if (!std::filesystem::exists(cacheFolder))
		// {
		// 	std::filesystem::create_directory(cacheFolder);
		// }
		//
		//
		// if (!std::filesystem::exists(shaderCache))
		// {
		// 	s_Registry = ShaderCompiler::CompileShadersFromPath(shaderPath.string());
		// 	std::filesystem::create_directory(shaderCache);
		// }
		//
		//
		// if (!s_Registry.empty())
		// {
		// 	ShaderCompiler::Dump(s_Registry, shaderCache);
		// }
		// else
		// {
		// 	s_Registry = ShaderCompiler::LoadFromDump(shaderCache);
		// }



	}

	bool ShaderRegistry::Exist(const std::string& path)
	{
		return s_RegistryShader.find(path) != s_RegistryShader.end();
	}

	void ShaderRegistry::SetPath(const std::string& path)
	{
	}

	Ref<Shader> ShaderRegistry::Load(const std::string& path)
	{
		if (!Exist(shaderPath.string() + "/" + path))
		{
			auto info = s_Registry.at(shaderPath.string() + "/" + path);
			auto shader = Shader::Create(info);
			s_RegistryShader[path] = shader;

			return shader;
		}

		return s_RegistryShader[path];
	}

	GraphicsShaders ShaderRegistry::LoadGraphicsShaders(const std::string& vtxPath,
		const std::string& fragPath)
	{

		std::string vtxPathWithExt = vtxPath;
		vtxPathWithExt.append(".vert");


		std::string fragPathWithExt = fragPath;
		fragPathWithExt.append(".frag");

		return 	{ Load(vtxPathWithExt), Load(fragPathWithExt)  };
	}

	GraphicsShaders ShaderRegistry::LoadGraphicsShaders(const std::string& path)
	{
		return 	LoadGraphicsShaders(path, path);
	}

	Ref<Shader> ShaderRegistry::Create(const std::string& path)
	{
		auto info = ShaderCompiler::Compile(path, "");
		s_Registry[path] = info;

		return Shader::Create(info);
	}
}
