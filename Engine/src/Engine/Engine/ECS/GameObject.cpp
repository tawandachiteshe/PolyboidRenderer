#include "boidpch.h"
#include "GameObject.h"

#include "Components.h"
#include "Engine/Engine/Scripting/ScriptingEngine.h"


namespace Polyboid
{
	GameObject::~GameObject()
	{
		
		auto& registry = GetWorld()->GetRegistry();
		registry.destroy(static_cast<entt::entity>(m_ID));
	}

	void GameObject::OnCreate()
	{
	}

	void GameObject::OnBeginPlay()
	{
		for (auto& script : m_MonoScripts)
		{
			script->InvokeMethod("OnBeginPlay");
		}
	}

	void GameObject::OnUpdate(float dt)
	{
		for (auto& script : m_MonoScripts)
		{
			script->InvokeMethod("OnUpdate", { &dt });
		}
	}

	void GameObject::OnEndPlay()
	{
		for (auto& script : m_MonoScripts)
		{
			script->InvokeMethod("OnEndPlay");
		}
	}

	void GameObject::OnDestroy()
	{
		
		auto& registry = GetWorld()->GetRegistry();
		registry.destroy(static_cast<entt::entity>(m_ID));
	}


	void GameObject::AttachScript(const std::string& monoKlassName)
	{

		auto klass = CreateRef<MonoClassInstance>(ScriptingEngine::FindClass(monoKlassName), monoKlassName);
		klass->SetField("gameObjectID", static_cast<uint64_t>(GetComponent<IDComponent>().id));
		klass->InvokeMethod("OnConstruct");
		m_MonoScripts.emplace_back(klass);
		m_MonoScriptsName.emplace_back(monoKlassName);
	}

	void GameObject::ReAttachScripts()
	{
		m_MonoScripts.clear();

		for (auto& name : m_MonoScriptsName)
		{
			auto klass = CreateRef<MonoClassInstance>(ScriptingEngine::FindClass(name), name);
			klass->SetField("gameObjectID", static_cast<uint64_t>(GetComponent<IDComponent>().id));
			klass->InvokeMethod("OnConstruct");
			m_MonoScripts.emplace_back(klass);
		}

	}

	void GameObject::ClearScripts()
	{
		m_MonoScripts.clear();
	}
}
