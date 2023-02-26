#pragma once

#include <any>
#include <spdlog/spdlog.h>
#include "Engine/Engine/Base.h"


extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoString MonoString;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoProperty MonoProperty;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef union _MonoError MonoError;
	typedef struct _MonoType MonoType;
}

namespace Polyboid
{

#define CPP_TO_MONO(x) &x


	template<typename T>
	T CastMonoResultAs(MonoObject* result)
	{
		return *static_cast<T*>(mono_object_unbox(result));
	}

	enum class ScriptingType
	{
		Float,
		Double,
		String,
		Boolean,
		Int32,
		Vector3
	};

	enum class AccessibilityType : uint8_t
	{
		None = 0,
		Private = (1 << 0),
		Internal = (1 << 1),
		Protected = (1 << 2),
		Public = (1 << 3)

	};

	class Accessibility
	{
	private:
		AccessibilityType m_Accessibility = AccessibilityType::None;
	public:
		Accessibility(AccessibilityType type) : m_Accessibility(type) {}

		operator uint8_t() const { return static_cast<uint8_t>(m_Accessibility); }
		operator AccessibilityType () const { return m_Accessibility; }


		Accessibility operator|=(const AccessibilityType& lhs)
		{
			uint8_t temp = static_cast<uint8_t>(m_Accessibility);
			temp |= static_cast<uint8_t>(lhs);
			m_Accessibility = static_cast<AccessibilityType>(temp);

			return static_cast<AccessibilityType>(temp);
		}

		friend bool operator==(const Accessibility& lhs, const Accessibility& rhs)
		{
			return lhs.m_Accessibility == rhs.m_Accessibility;
		}
	};

	struct ScriptingData
	{

		std::unordered_map<std::string, ScriptingType> TypeMap = {
			{ "System.Single", ScriptingType::Float },
			{ "System.Double", ScriptingType::Double },
			{ "System.Int32", ScriptingType::Int32 },
			{ "System.String", ScriptingType::String },
			{ "System.Boolean", ScriptingType::Boolean },
			{ "Polyboid.Vector3", ScriptingType::Vector3 },
		};

		MonoAssembly* EngineAssembly = nullptr;
		MonoAssembly* AppAssembly = nullptr;

		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		std::vector<std::string> Classes;
		std::unordered_map<std::string, MonoClass*> MonoClasses;
		std::unordered_map<std::string, std::vector<std::pair<std::string, ScriptingType>>> ClassFields;
	};

	struct ScriptSettings
	{
		bool MonoDebug = false;
	};

	class ScriptingEngine
	{
	private:

		static Unique<ScriptingData> s_Data;

		static void InitMono();


		static void InitAppDomain();

		static MonoAssembly* LoadAssembly(const std::string& assemblyPath);

		static void LoadAssemblies();

		static void PrintAssemblyType(MonoAssembly* assembly);

		static void GetClassesInfo();

		

	public:


		static void Init();

		static void ReloadAssembly();

		static  std::vector<std::string>& GetClasses() { return s_Data->Classes; }
		static  std::vector < std::pair<std::string, ScriptingType>>& GetClassFields(const std::string& className) { return s_Data->ClassFields.at(className); }

		static void ShutDown();
		static MonoString* ToMonoString(const std::string& str);
		static MonoImage* GetMonoImage(MonoAssembly* assembly);
		static MonoClass* FindClass(const std::string& fullName);
		static MonoAssembly* GetAppAssembly() {
			return s_Data->AppAssembly;
		}

		static MonoObject* CreateMonoClassInstance(MonoClass* klass, bool callDefaultConstructor = true);
		static MonoObject* CreateMonoClassInstance(const char* namespaceName, const char* className, MonoAssembly* assembly);

		static MonoClass* GetMonoClass(const char* namespaceName, const char* className, MonoAssembly* assembly);
		static MonoClass* GetMonoClassFromInstance(MonoObject* instance);
		static MonoMethod* GetMonoMethod(MonoClass* instanceClass, const char* methodName, int paramCount = 0);
		static MonoObject* InvokeMonoMethod(MonoObject* instance, const char* methodName , int paramCount = 0, void** params = nullptr);
		static MonoObject* InvokeMonoMethod(MonoObject* instance, const char* methodName, std::initializer_list<void*> params);
		static MonoClassField* GetMonoClassField(MonoObject* instance, const char* name);
		static MonoProperty* GetMonoClassProperty(MonoObject* instance, const char* name);
		static AccessibilityType GetFieldAccessibility(MonoClassField* field);
		static AccessibilityType GetPropertyAccessibility(MonoProperty* property);
		static bool CheckMonoError(MonoError& error);
		static std::string MonoStringToUTF8(MonoString* monoString);
		static void SetMonoFieldValue(MonoObject* instance, MonoClassField* field, void* value);
		static void GetMonoFieldValue(MonoObject* instance, MonoClassField* field, void* value);

		static void* GetMonoValueToCPP(MonoObject* instance);
		static int GetClassFieldNumCount(MonoObject* object);
		static MonoClassField* GetClassFields(MonoObject* object, void* iterator);
		static const char* GetMonoClassFieldName(MonoClassField* field);
		static MonoType* GetMonoClassFieldType(MonoClassField* field);
		static const char* GetMonoClassFieldTypeName(MonoType* type);



		//templelates here

		//Experimental
		template<typename T>
		static  std::function<T> GetFunctionPointer(MonoMethod* method)
		{
			return static_cast<std::function<T>>(mono_method_get_unmanaged_thunk(method));
		}

		
		static void* GetFunctionPointer(MonoObject* instance, const std::string& name, int paramCount = 0);


		template<typename T>
		static void SetMonoClassInstancedProperty(MonoObject* instance, const char* propertyName, std::initializer_list<void*> params = {})
		{
			auto property = GetMonoClassProperty(instance, propertyName);

			if (property == nullptr)
			{
				return;
			}

			if (params.size() > 0)
			{
				std::vector<void*> paramsArray(params);
				mono_property_set_value(property, instance, paramsArray.data(), nullptr);
			}

		}

		template<typename T>
		static T GetMonoClassInstancedProperty(MonoObject* instance, const char* propertyName, std::initializer_list<void*> params = {})
		{
			auto property = GetMonoClassProperty(instance, propertyName);

			if (property == nullptr)
			{
				return 0;
			}

			if (params.size() > 0)
			{
				std::vector<void*> paramsArray(params);
				auto value = mono_property_get_value(property, instance, paramsArray.data(), nullptr);
				return  CastMonoResultAs<T>(value);
			}

			auto value = mono_property_get_value(property, instance, nullptr, nullptr);
			return  CastMonoResultAs<T>(value);

		}

		template<typename T>
		static T GetMonoClassInstanceField(MonoObject* instance, const char* fieldName)
		{
			T value;
			auto field = GetMonoClassField(instance, fieldName);

			if (field == nullptr)
			{
				return T{};
			}

			GetMonoFieldValue(instance, field, &value);
			return value;
		}

		template<typename T>
		static void SetMonoClassInstanceField(MonoObject* instance, const char* fieldName, T value)
		{
			auto field = GetMonoClassField(instance, fieldName);

			if (field == nullptr)
			{
				return;
			}

			SetMonoFieldValue(instance, field, &value);
			

		}

		template<>
		static void SetMonoClassInstanceField(MonoObject* instance, const char* fieldName, MonoString* value)
		{
			auto field = GetMonoClassField(instance, fieldName);

			if (field == nullptr)
			{
				return;
			}

			SetMonoFieldValue(instance, field, value);
		}


		
	};

	template<>
	inline std::string CastMonoResultAs(MonoObject* result)
	{
		return ScriptingEngine::MonoStringToUTF8(static_cast<MonoString*>(ScriptingEngine::GetMonoValueToCPP(result)));
	}

	class MonoClassInstance
	{
	private:
		MonoObject* m_Instance = nullptr;
		std::string m_ClassName;

	public:

		std::string& GetMonoClassName() { return m_ClassName; }

		std::vector<std::pair<std::string, ScriptingType>> GetFields() { return ScriptingEngine::GetClassFields(m_ClassName); }

		operator MonoObject* () const { return m_Instance; }

		MonoClassInstance(MonoClass* klass, const std::string& className = "hehe") : m_ClassName(className)
		{
			m_Instance = ScriptingEngine::CreateMonoClassInstance(klass);
			
		}

		MonoClassInstance(const std::string& nameSpaceName, const std::string& className, MonoAssembly* assembly) 
		{
			std::string tempClass = className;
			std::string tempName = nameSpaceName;

			m_ClassName = tempName.append(".").append(className);
			

			m_Instance = ScriptingEngine::CreateMonoClassInstance(nameSpaceName.c_str(), className.c_str(), assembly);
			
		}

		template<typename T>
		T GetField(const std::string& fieldName)
		{
			return ScriptingEngine::GetMonoClassInstanceField<T>(m_Instance, fieldName.c_str());
		}

		template<typename T>
		void SetField(const std::string& fieldName, T value)
		{
			ScriptingEngine::SetMonoClassInstanceField<T>(m_Instance, fieldName.c_str(), value);
		}


		template<>
		void SetField(const std::string& fieldName, std::string value)
		{

			ScriptingEngine::SetMonoClassInstanceField<MonoString*>(m_Instance, fieldName.c_str(), ScriptingEngine::ToMonoString(value));
		}

		template<typename T>
		T GetProperty(const std::string& fieldName, std::initializer_list<void*> params = {})
		{
			return ScriptingEngine::GetMonoClassInstancedProperty<T>(m_Instance, fieldName.c_str(), params);
		}

		template<typename T>
		void SetProperty(const std::string& fieldName, std::initializer_list<void*> params = {})
		{
			ScriptingEngine::SetMonoClassInstancedProperty<T>(m_Instance, fieldName.c_str(), params);
		}

		void* GetMethodFunctionPointer(const std::string& name, int paramsCount = 0)
		{
			return  ScriptingEngine::GetFunctionPointer(m_Instance, name, paramsCount);
		}


		void InvokeMethod(const std::string& methodName, std::initializer_list<void*> params = {})
		{
			ScriptingEngine::InvokeMonoMethod(m_Instance, methodName.c_str(), params);
		}

		template<typename ReturnType>
		ReturnType InvokeMethod(const std::string& methodName, std::initializer_list<void*> params = {})
		{
			return CastMonoResultAs<ReturnType>(ScriptingEngine::InvokeMonoMethod(m_Instance, methodName.c_str(), params));
		}

	};


}
