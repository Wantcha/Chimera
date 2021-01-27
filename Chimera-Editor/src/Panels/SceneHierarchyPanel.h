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
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}