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
}