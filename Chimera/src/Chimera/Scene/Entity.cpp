#include "cmpch.h"
#include "Entity.h"
#include "Components.h"

namespace Chimera
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
	std::string Entity::GetName()
	{
		return GetComponent<TagComponent>().Name;
	}
	void Entity::SetName(std::string name)
	{
		GetComponent<TagComponent>().Name = name;
	}
	bool Entity::IsEnabled()
	{
		return GetComponent<TagComponent>().Enabled;
	}
	void Entity::SetEnabled(bool enabled)
	{
		GetComponent<TagComponent>().Enabled = enabled;

		if (enabled)
		{
			if (this->HasComponent<Body2DComponent>())
			{
				auto& body = this->GetComponent<Body2DComponent>();
				body.Body->SetEnabled(true);
			}
		}
		else
		{
			if (this->HasComponent<Body2DComponent>())
			{
				auto& body = this->GetComponent<Body2DComponent>();
				body.Body->SetEnabled(false);
			}
		}
	}
}