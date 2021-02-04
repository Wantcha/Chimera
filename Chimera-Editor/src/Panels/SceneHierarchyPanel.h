#pragma once

#include "Chimera/Scene/Scene.h"
#include "Chimera/Core/Log.h"
#include "Chimera/Core/Core.h"
#include "Chimera/Scene/Entity.h"

namespace Chimera
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(int selectionID) { 
			if (selectionID >= 0 && selectionID < m_Context->m_Registry.size())
			{
				Entity entity = { m_Context->m_Registry.data()[selectionID], m_Context.get() }; 
				m_SelectionContext = entity;
			}
			 }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}