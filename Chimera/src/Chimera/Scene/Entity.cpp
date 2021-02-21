#include "cmpch.h"
#include "Entity.h"

namespace Chimera
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}