#include "boidpch.h"
#include "ShaderCompiler.h"


#include <execution>
#include <spdlog/spdlog.h>
#include "taskflow/taskflow.hpp"

#include "ShadercIncluder.h"
#include "Engine/Engine/Debug/Timer.h"
#include "Engine/Renderer/Shader.h"
#include "shaderc/shaderc.hpp"
#include "spirv_cross/spirv_cross.hpp"
#include "spirv_cross/spirv_reflect.hpp"
#include "nlohmann/json.hpp"
#include "hashpp/hashpp.h"

#include <pods/pods.h>
#include <pods/msgpack.h>
#include <pods/streams.h>
#include "msgpack.hpp"

namespace Polyboid
{
	struct ShaderCacheInfo
	{
		std::vector<uint32_t> Spirv;


		PODS_SERIALIZABLE(
			PODS_MDR(Spirv)
		)
	};

	shaderc_shader_kind GetShaderKindFromExt(const std::string& shaderExt)
	{
		if (shaderExt == ".vert")
		{
			return shaderc_vertex_shader;
		}

		if (shaderExt == ".frag")
		{
			return shaderc_fragment_shader;
		}

		if (shaderExt == ".comp")
		{
			return shaderc_compute_shader;
		}

		return shaderc_anyhit_shader;
	}

	ShaderType GetEngineShaderType(const std::string& shaderExt)
	{
		if (shaderExt == ".vert")
		{
			return ShaderType::Vertex;
		}
		else if (shaderExt == ".frag")
		{
			return ShaderType::Fragment;
		}
		else if (shaderExt == ".comp")
		{
			return ShaderType::Compute;
		}

		return ShaderType::Vertex;
	}


	ShaderBinaryAndInfo ShaderCompiler::Compile(const std::filesystem::path& path, const std::string& rootPath)
	{
		tf::Executor executor;

		auto shaderFileExt = path.extension().string();
		const auto parentPath = path.parent_path();

		auto engineShaderType = GetEngineShaderType(shaderFileExt);

		std::ifstream shaderFile(path);
		std::string contents((std::istreambuf_iterator<char>(shaderFile)),
		                     std::istreambuf_iterator<char>());

		//Render API here for shader stuff

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		//TODO: Make this an option
		//options.SetSourceLanguage(shaderc_source_language::shaderc_source_language_hlsl);

		//For now i support only opengl
		//TODO: make this api agnostic
		options.SetTargetEnvironment(shaderc_target_env::shaderc_target_env_opengl, 0);

		//TODO: make this in more robust
		options.SetOptimizationLevel(shaderc_optimization_level::shaderc_optimization_level_zero);
		//only for vulkan
		//options.SetTargetSpirv(shaderc_targe)
		options.SetGenerateDebugInfo();


		std::unique_ptr<shaderc::CompileOptions::IncluderInterface> includer = std::make_unique<
			ShadercIncluder>(rootPath);
		options.SetIncluder(std::move(includer));

		shaderc::PreprocessedSourceCompilationResult preResults;
		executor.async([&]()
		{
			preResults = compiler.PreprocessGlsl(contents, GetShaderKindFromExt(shaderFileExt), path.string().c_str(),
			                                     options);
		}).wait();


		std::vector<char> preSource;
		if (preResults.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			spdlog::warn("shader compilation failed: {}", preResults.GetErrorMessage());
		}
		else
		{
			preSource = std::vector(preResults.begin(), preResults.end());

			//NULL termination
			preSource.push_back(0);
		}

		std::vector<uint32_t> shaderSpv;

		if (!preSource.empty())
		{
			auto result = compiler.CompileGlslToSpv(std::string(preSource.data()), GetShaderKindFromExt(shaderFileExt),
			                                        path.string().c_str(), options);

			if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				spdlog::error("{}", result.GetErrorMessage());
			}
			else
			{
				if (result.GetNumWarnings())
				{
					spdlog::warn("Shader warnings: {}", result.GetNumWarnings());
					spdlog::warn("{}", result.GetErrorMessage());
				}

				shaderSpv = std::vector(result.cbegin(), result.cend());
			}
		}

		ShaderBinaryAndInfo info;

		if (!shaderSpv.empty())
		{
			info.Spirv = shaderSpv;
			info.type = static_cast<uint8_t>(engineShaderType);
			info.filePath = path.generic_string();


			spirv_cross::CompilerReflection reflection(shaderSpv);
			auto res = reflection.get_shader_resources();
			auto reflectionJson = reflection.compile();

			//Reflect(reflectionJson, info);
			info.shaderReflect = reflectionJson;
			info.spirvChecksum = SpirvCheckSum(shaderSpv);

			Reflect(reflectionJson, info.reflectionInfo);

			//auto allDumpData = reflectionJson.dump(4);
		}

		return info;
	}

	void ShaderCompiler::Reflect(const std::string& shaderReflectJson, ReflectionInfo& info)
	{
		auto reflectionJson = nlohmann::json::parse(shaderReflectJson);

		if (reflectionJson.contains("images"))
		{
			auto images = reflectionJson["images"];

			for (auto& image : images)
			{
				//TODO: add format
				ShaderImageInfo imageInfo;
				imageInfo.Binding = image["binding"].get<uint32_t>();
				imageInfo.Name = image["name"].get<std::string>();
				imageInfo.Set = image["set"].get<uint32_t>();

				info.images[imageInfo.Name] = imageInfo;
			}
		}




		if (reflectionJson.contains("textures"))
		{
			auto textures = reflectionJson["textures"];

			for (auto& texture : textures)
			{
				ShaderImageInfo shaderInfo;

				shaderInfo.Binding = texture["binding"].get<uint32_t>();
				shaderInfo.Name = texture["name"].get<std::string>();
				shaderInfo.Set = texture["set"].get<uint32_t>();

				info.textures[shaderInfo.Name] = (shaderInfo);
			}
		}

		if (reflectionJson.contains("ubos"))
		{
			auto ubos = reflectionJson["ubos"];

			for (auto& ubo : ubos)
			{
				UniformBufferInfo shaderInfo;

				shaderInfo.Binding = ubo["binding"].get<uint32_t>();
				shaderInfo.Name = ubo["name"].get<std::string>();
				shaderInfo.Set = ubo["set"].get<uint32_t>();
				shaderInfo.blockSize = ubo["block_size"].get<uint32_t>();

				info.ubos[shaderInfo.Name] = (shaderInfo);
			}
		}

		if (reflectionJson.contains("ssbos"))
		{
			auto ssbos = reflectionJson["ssbos"];

			for (auto& ssbo : ssbos)
			{
				ShaderStorageInfo shaderInfo;

				shaderInfo.Binding = ssbo["binding"].get<uint32_t>();
				shaderInfo.Name = ssbo["name"].get<std::string>();
				shaderInfo.Set = ssbo["set"].get<uint32_t>();
				shaderInfo.blockSize = ssbo["block_size"].get<uint32_t>();
				if (ssbo.contains("readonly"))
				{
					shaderInfo.isReadOnly = ssbo["readonly"].get<bool>();
				}

				info.ssbos[shaderInfo.Name] = shaderInfo;
			}
		}
	}


	static bool IsFileShader(const std::filesystem::path& filePath)
	{
		auto fileExt = filePath.extension().string();
		return fileExt == ".vert" || fileExt == ".frag" || fileExt == ".comp";
	}


	std::unordered_map<std::string, ShaderBinaryAndInfo> ShaderCompiler::CompileShadersFromPath(
		const std::filesystem::path& directoryPath)
	{
		tf::Executor executor;
		tf::Taskflow taskflow("Compile Shaders");

		Timer t;
		static std::vector<ShaderBinaryAndInfo> shaderResults;

		std::vector<std::string> filePaths;
		std::filesystem::recursive_directory_iterator files(directoryPath);
		for (auto& file : files)
		{
			if (file.is_regular_file())
			{
				if (IsFileShader(file.path()))
				{
					auto filePath = file.path().generic_string();
					filePaths.push_back(filePath);
				}
			}
		}

	
		std::unordered_map<std::string, ShaderBinaryAndInfo> infos;
		infos.reserve(filePaths.size());
		shaderResults.resize(filePaths.size());

		tf::Task task = taskflow.for_each(filePaths.begin(), filePaths.end(), [&](std::string& file)
		{
			auto info = Compile(file, directoryPath.generic_string());
			infos[file] = info;
		});

		executor.run(taskflow).wait();


		spdlog::info("Shader compilation took {} ms. Compiled {} shaders", t.ElapsedMillis(), infos.size());

		return infos;
	}

	bool ShaderCompiler::Dump(const std::unordered_map<std::string, ShaderBinaryAndInfo>& shaderBinaries,
	                          const std::filesystem::path& cachePath)
	{
		tf::Executor executor;
		tf::Taskflow taskflow;

		Timer t;


		if (std::filesystem::exists(cachePath))
		{
			taskflow.for_each(shaderBinaries.cbegin(), shaderBinaries.cend(), [&](const auto& entry)
			{
				auto& [path, shaderInfo] = entry;
				std::filesystem::path fileSystemPath = path;
				auto filename = fileSystemPath.stem();
				auto fileExt = fileSystemPath.extension();
				auto cacheExtension = ".spv";

				auto cacheFilePath = (cachePath / (filename.string() + cacheExtension + fileExt.string())).
					generic_string();

				std::ofstream outStream(cacheFilePath, std::ios::binary | std::ios::out);
				pods::OutputStream out(outStream);

				
				pods::MsgPackSerializer<decltype(out)> serializer(out);
				if (serializer.save(shaderInfo) == pods::Error::NoError)
				{
				}


				outStream.close();
			});
		}

		executor.run(taskflow).wait();

		spdlog::info("Dumping {} shaders took {} ms", shaderBinaries.size(), t.ElapsedMillis());

		return true;
	}

	ShaderCompiler::ShaderBinaryMap ShaderCompiler::LoadFromDump(const std::filesystem::path& cachePath)
	{


		tf::Executor executor;
		tf::Taskflow taskflow;

		Timer t;
		std::vector<std::string> cacheFilePaths;
		for (auto& entry : std::filesystem::directory_iterator(cachePath))
		{
			cacheFilePaths.push_back(entry.path().generic_string());
		}

		ShaderBinaryMap shaderMap;
		shaderMap.reserve(cacheFilePaths.size());

		taskflow.for_each(cacheFilePaths.begin(), cacheFilePaths.end(), [&](std::string& file)
		{

			std::filesystem::path fileSystemPath = file;
			ShaderBinaryAndInfo binaryInfo;

			std::ifstream buffer(file, std::ios::binary | std::ios::in);


			pods::InputStream in(buffer);
			pods::MsgPackDeserializer<decltype(in)> deserializer(in);

			auto error = deserializer.load(binaryInfo);

			if (error == pods::Error::NoError)
			{
				shaderMap[binaryInfo.filePath] = binaryInfo;
			}

			buffer.close();
			
		});

		executor.run(taskflow).wait();

		spdlog::info("Loading {} shaders from cache took {} ms", shaderMap.size(), t.ElapsedMillis());

		return shaderMap;
	}

	std::string ShaderCompiler::SpirvCheckSum(const std::vector<uint32_t>& spirv)
	{
		auto bytes = reinterpret_cast<const char*>(spirv.data());
		std::string stringBytes;
		stringBytes.resize(spirv.size() * sizeof(uint32_t));
		std::memcpy(&stringBytes[0], bytes, spirv.size() * sizeof(uint32_t));

		auto md5 = hashpp::MD::MD5();
		return md5.getHash(stringBytes);
	}
}
