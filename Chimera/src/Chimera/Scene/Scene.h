#pragma once

#include "entt.hpp"
#include "box2d/box2d.h"
#include "Chimera/Core/Timestep.h"
#include "Chimera/Renderer/EditorCamera.h"
#include "Chimera/Physics/Physics2DDebugDraw.h"
#include "Chimera/Physics/RigidBody2D.h"
#include "Chimera/Physics/ContactListener2D.h"

namespace Chimera
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		template<typename T>
		void OnComponentRemoved(Entity entity, T& component);

		entt::registry m_Registry;

		Scope<b2World> m_World;
		std::stack<b2Body*> m_DestructionStack;
		//std::stack<b2Fixture*>
		//Physics2DDebugDraw m_DebugDraw;
		ContactListener2D m_ContactListener;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_EditColliders = false;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		friend class RigidBody2D;
	};
}