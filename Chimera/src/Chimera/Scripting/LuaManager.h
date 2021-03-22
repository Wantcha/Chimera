#pragma once
#include <sol/sol.hpp>
#include "Chimera/Core/Timestep.h"
#include "Chimera/Scene/Entity.h"

namespace Chimera
{
#define REGISTER_COMPONENT_WITH_ECS(curLuaState, component) \
	{ \
		/*auto entity_type = curLuaState["Entity"].get_or_create<sol::usertype<Entity>>(); */\
		entity_type.set_function("Add" #component, &Entity::AddComponent<component>); \
		entity_type.set_function("Remove" #component, &Entity::RemoveComponent<component>); \
		entity_type.set_function("Get" #component, &Entity::GetComponent<component>); \
	}

	class Scene;
	class LuaScriptComponent;

	class LuaManager
	{
	public:
		LuaManager();
		~LuaManager();

		void Init(Scene* scene);
		void InitBindings();
		void InitScripts();
		void UpdateScripts(Timestep ts);

		void BindECS();
		void BindInput();
		void BindScene();
		void BindApp();
		void BindLog();
		void BindPhysics();

		void BindMath();

		sol::state& GetState()
		{
			return m_State;
		}

		static LuaManager& Get()
		{
			if (!s_Instance)
				s_Instance = new LuaManager();

			return *s_Instance;
		}

		Entity GetEntityFromScript(const LuaScriptComponent& lsc)
		{
			Entity entity = { entt::to_entity(m_CurrentScene->m_Registry, lsc), m_CurrentScene };
			return entity;
		}

	private:
		sol::state m_State;
		Scene* m_CurrentScene = nullptr;
		static LuaManager* s_Instance;
	};
}
