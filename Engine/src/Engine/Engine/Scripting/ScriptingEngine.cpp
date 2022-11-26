#include "boidpch.h"
#include "ScriptingEngine.h"

#include <spdlog/spdlog.h>

#include "ScriptingInternals.hpp"
#include "glm/vec3.hpp"
#include "mono/metadata/assembly.h"
#include "mono/metadata/appdomain.h"
#include "mono/metadata/attrdefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include <mono/metadata/object.h>
#include "mono/metadata/object.h"
#include "mono/jit/jit.h"

namespace Polyboid
{
	//TODO: Handle errors

	Unique<ScriptingData> ScriptingEngine::s_Data = std::make_unique<ScriptingData>();

	namespace Utils
	{
		char* ReadBytes(const std::string& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}
	}

	void ScriptingEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		if (false)
		{
			const char* argv[2] = {
				"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
				"--soft-breakpoints"
			};

			mono_jit_parse_options(2, (char**)argv);
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}


		MonoDomain* rootDomain = mono_jit_init("PolyboidRuntime");


		if (rootDomain == nullptr)
		{
			return;
		}

		s_Data->RootDomain = rootDomain;

		s_Data->AppDomain = mono_domain_create_appdomain("PolyboidAppDomain", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		if (false)
			mono_debug_domain_create(s_Data->RootDomain);

		// mono_thread_set_main(mono_thread_current());

		s_Data->Classes.reserve(100);
	}

	MonoAssembly* ScriptingEngine::LoadAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = Utils::ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			spdlog::info("Error {}", errorMessage);
			// Log some error message using the errorMessage data
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	typedef void (*PrintAll)(MonoObject* obj);


	void ScriptingEngine::LoadAssemblies()
	{
		InitInternalCalls();

		s_Data->EngineAssembly = LoadAssembly("Resources/Scripts/PolyboidManaged.dll");
		s_Data->AppAssembly = LoadAssembly("Project/Binaries/Sandbox.dll");

		PrintAssemblyType(s_Data->EngineAssembly);


		// MonoClassInstance m_Instance("Polyboid", "Tawanda");
		// m_Instance.InvokeMethod("PrintAll");
		// m_Instance.SetField<float>("health", 200);
		// m_Instance.SetField<std::string>("m_Name", "TAwand ais cool templan");
		// m_Instance.InvokeMethod("PrintAll");

		GetClassesInfo();

		//InvokeMonoMethod(m_Instance, "SayName", 3, params);
	}

	void ScriptingEngine::PrintAssemblyType(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			spdlog::info("{}.{}", nameSpace, name);
		}
	}

	void ScriptingEngine::GetClassesInfo()
	{
		auto image = GetMonoImage(s_Data->AppAssembly);
		const MonoTableInfo* typeDefs = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefs);
		auto GameObjectClass = GetMonoClass("Polyboid", "GameObject", s_Data->EngineAssembly);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];

			mono_metadata_decode_row(typeDefs, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			auto monoClass = GetMonoClass(nameSpace, className, s_Data->AppAssembly);

			if (monoClass == GameObjectClass)
			{
				continue;
			}

			bool isGameObject = mono_class_is_subclass_of(monoClass, GameObjectClass, false);
			
			if (!isGameObject)
				continue;

			//cache them here...
			s_Data->Classes.push_back(fullName);
			s_Data->MonoClasses[fullName] = monoClass;


			int fieldCount = mono_class_num_fields(monoClass);
			spdlog::info("{} has {} fields", className, fieldCount);
			void* iterator = nullptr;

			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				//Cache fields in here

				MonoType* type = mono_field_get_type(field);
				auto typeName = mono_type_get_name(type);

				if (GetFieldAccessibility(field) == AccessibilityType::Public)
				{
					spdlog::info("Type of field {} is {}", fieldName, typeName);
				}
			}
		}
	}

	void ScriptingEngine::ShutDown()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	MonoString* ScriptingEngine::ToMonoString(const std::string& str)
	{
		MonoString* name = mono_string_new(mono_domain_get(), str.c_str());
		return name;
	}

	MonoImage* ScriptingEngine::GetMonoImage(MonoAssembly* assembly)
	{
		return mono_assembly_get_image(assembly);
	}

	MonoClass* ScriptingEngine::FindClass(const std::string& fullName)
	{
		return s_Data->MonoClasses.at(fullName);
	}

	MonoObject* ScriptingEngine::CreateMonoClassInstance(MonoClass* klass, bool callDefaultConstructor)
	{
		MonoObject* classInstance = mono_object_new(s_Data->AppDomain, klass);

		if (classInstance == nullptr)
		{
			return nullptr;
		}

		if (callDefaultConstructor)
		{
			mono_runtime_object_init(classInstance);
		}

		return classInstance;
	}

	MonoObject* ScriptingEngine::CreateMonoClassInstance(const char* namespaceName, const char* className,
	                                                     MonoAssembly* assembly)
	{
		auto klass = GetMonoClass(namespaceName, className, assembly);

		if (klass == nullptr)
		{
			return nullptr;
		}

		return CreateMonoClassInstance(klass, true);
	}


	MonoClass* ScriptingEngine::GetMonoClass(const char* namespaceName, const char* className, MonoAssembly* assembly)
	{
		MonoImage* image = GetMonoImage(assembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName, className);

		if (klass == nullptr)
		{
			return nullptr;
		}

		return klass;
	}

	MonoClass* ScriptingEngine::GetMonoClassFromInstance(MonoObject* instance)
	{
		MonoClass* instanceClass = mono_object_get_class(instance);
		if (instanceClass == nullptr)
		{
			return nullptr;
		}

		return instanceClass;
	}

	MonoMethod* ScriptingEngine::GetMonoMethod(MonoClass* instanceClass, const char* methodName, int paramCount)
	{
		MonoMethod* method = mono_class_get_method_from_name(instanceClass, methodName, paramCount);

		if (method == nullptr)
		{
			return nullptr;
		}


		return method;
	}

	MonoObject* ScriptingEngine::InvokeMonoMethod(MonoObject* instance, const char* methodName, int paramCount,
	                                              void** params)
	{
		MonoObject* exception = nullptr;

		MonoClass* klass = mono_object_get_class(instance);

		MonoMethod* method = GetMonoMethod(klass, methodName, paramCount);

		MonoObject* object = nullptr;

		if (paramCount > 0)
		{
			object = mono_runtime_invoke(method, instance, params, &exception);
		}
		else
		{
			object = mono_runtime_invoke(method, instance, nullptr, &exception);
		}

		if (exception)
		{
			return nullptr;
		}


		return object;
	}

	MonoObject* ScriptingEngine::InvokeMonoMethod(MonoObject* instance, const char* methodName,
	                                              std::initializer_list<void*> params)
	{
		const auto paramsCount = params.size();

		std::vector<void*> paramsArray(params);


		return InvokeMonoMethod(instance, methodName, paramsCount, paramsArray.data());
	}

	MonoClassField* ScriptingEngine::GetMonoClassField(MonoObject* instance, const char* name)
	{
		auto klass = GetMonoClassFromInstance(instance);

		if (klass == nullptr)
		{
			return nullptr;
		}

		auto field = mono_class_get_field_from_name(klass, name);

		if (field == nullptr)
		{
			return field;
		}


		return field;
	}

	MonoProperty* ScriptingEngine::GetMonoClassProperty(MonoObject* instance, const char* name)
	{
		auto klass = GetMonoClassFromInstance(instance);

		if (klass == nullptr)
		{
			return nullptr;
		}

		auto property = mono_class_get_property_from_name(klass, name);

		if (property == nullptr)
		{
			return property;
		}


		return property;
	}

	AccessibilityType ScriptingEngine::GetFieldAccessibility(MonoClassField* field)
	{
		Accessibility accessibility = AccessibilityType::None;
		uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

		switch (accessFlag)
		{
		case MONO_FIELD_ATTR_PRIVATE:
			{
				accessibility = AccessibilityType::Private;
				break;
			}
		case MONO_FIELD_ATTR_FAM_AND_ASSEM:
			{
				accessibility |= AccessibilityType::Protected;
				accessibility |= AccessibilityType::Internal;
				break;
			}
		case MONO_FIELD_ATTR_ASSEMBLY:
			{
				accessibility = AccessibilityType::Internal;
				break;
			}
		case MONO_FIELD_ATTR_FAMILY:
			{
				accessibility = AccessibilityType::Protected;
				break;
			}
		case MONO_FIELD_ATTR_FAM_OR_ASSEM:
			{
				accessibility |= AccessibilityType::Private;
				accessibility |= AccessibilityType::Protected;
				break;
			}
		case MONO_FIELD_ATTR_PUBLIC:
			{
				accessibility = AccessibilityType::Public;
				break;
			}
		}

		return accessibility;
	}

	AccessibilityType ScriptingEngine::GetPropertyAccessibility(MonoProperty* property)
	{
		Accessibility accessibility = AccessibilityType::None;

		// Get a reference to the property's getter method
		MonoMethod* propertyGetter = mono_property_get_get_method(property);
		if (propertyGetter != nullptr)
		{
			// Extract the access flags from the getters flags
			uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

			switch (accessFlag)
			{
			case MONO_FIELD_ATTR_PRIVATE:
				{
					accessibility = AccessibilityType::Private;
					break;
				}
			case MONO_FIELD_ATTR_FAM_AND_ASSEM:
				{
					accessibility |= AccessibilityType::Protected;
					accessibility |= AccessibilityType::Internal;
					break;
				}
			case MONO_FIELD_ATTR_ASSEMBLY:
				{
					accessibility = AccessibilityType::Internal;
					break;
				}
			case MONO_FIELD_ATTR_FAMILY:
				{
					accessibility = AccessibilityType::Protected;
					break;
				}
			case MONO_FIELD_ATTR_FAM_OR_ASSEM:
				{
					accessibility |= AccessibilityType::Private;
					accessibility |= AccessibilityType::Protected;
					break;
				}
			case MONO_FIELD_ATTR_PUBLIC:
				{
					accessibility = AccessibilityType::Public;
					break;
				}
			}
		}

		// Get a reference to the property's setter method
		MonoMethod* propertySetter = mono_property_get_set_method(property);
		if (propertySetter != nullptr)
		{
			// Extract the access flags from the setters flags
			uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
			if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
				accessibility = AccessibilityType::Private;
		}
		else
		{
			accessibility = AccessibilityType::Private;
		}

		return accessibility;
	}

	bool ScriptingEngine::CheckMonoError(MonoError& error)
	{
		bool hasError = !mono_error_ok(&error);
		if (hasError)
		{
			unsigned short errorCode = mono_error_get_error_code(&error);
			const char* errorMessage = mono_error_get_message(&error);
			printf("Mono Error!\n");
			printf("\tError Code: %hu\n", errorCode);
			printf("\tError Message: %s\n", errorMessage);
			mono_error_cleanup(&error);
		}
		return hasError;
	}

	std::string ScriptingEngine::MonoStringToUTF8(MonoString* monoString)
	{
		if (monoString == nullptr || mono_string_length(monoString) == 0)
			return "";

		MonoError error;
		char* utf8 = mono_string_to_utf8_checked(monoString, &error);
		if (CheckMonoError(error))
			return "";
		std::string result(utf8);
		mono_free(utf8);
		return result;
	}

	void ScriptingEngine::SetMonoFieldValue(MonoObject* instance, MonoClassField* field, void* value)
	{
		mono_field_set_value(instance, field, value);
	}

	void* ScriptingEngine::GetMonoValueToCPP(MonoObject* instance)
	{
		return mono_object_unbox(instance);
	}

	void* ScriptingEngine::GetFunctionPointer(MonoObject* instance, const std::string& name, int paramCount)
	{
		auto klass = GetMonoClassFromInstance(instance);
		auto method = GetMonoMethod(klass, name.c_str(), paramCount);
		return mono_method_get_unmanaged_thunk(method);
	}


	void ScriptingEngine::Init()
	{
		InitMono();
		LoadAssemblies();
	}
}
