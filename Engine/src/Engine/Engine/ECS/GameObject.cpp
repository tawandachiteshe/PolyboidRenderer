#include "boidpch.h"
#include "GameObject.h"


namespace Polyboid
{
	GameObject::~GameObject()
	{
		auto& registry = GetWorld()->GetRegistry();
		registry.destroy(static_cast<entt::entity>(m_ID));
	}

	void GameObject::OnDestroy()
	{
		auto& registry = GetWorld()->GetRegistry();
		registry.destroy(static_cast<entt::entity>(m_ID));
	}
}
