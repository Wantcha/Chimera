#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Chimera
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(std::nullptr_t) :m_EntityHandle(entt::null), m_Scene(nullptr) {};
		Entity(const Entity& other) = default;
		~Entity() { m_EntityHandle = entt::null; m_Scene = nullptr; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			CM_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			CM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			CM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			m_Scene->OnComponentRemoved<T>(*this, GetComponent<T>());
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		bool IsValid()
		{
			return m_Scene->m_Registry.valid(m_EntityHandle);
		}

		void SetNullScene()
		{
			m_Scene = nullptr;
		}

		std::string GetName();
		void SetName(std::string name);

		bool IsEnabled();
		void SetEnabled(bool enabled);

		void MakeRootNode()
		{
			m_Scene->PushRootEntity(*this);
		}

		void RemoveRootNode()
		{
			m_Scene->RemoveRootEntity(*this);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator std::nullptr_t() const { return m_EntityHandle == entt::null ? nullptr : nullptr ; }
		operator entt::entity() const { return m_EntityHandle; }
		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

		Entity& operator=(std::nullptr_t) noexcept { m_EntityHandle = entt::null; m_Scene = nullptr ; return *this; }
		bool operator==(std::nullptr_t) const { return m_EntityHandle == entt::null || m_Scene == nullptr; }
		bool operator!=(std::nullptr_t) const { return !(m_EntityHandle == entt::null)/* && m_Scene == nullptr)*/; }
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
