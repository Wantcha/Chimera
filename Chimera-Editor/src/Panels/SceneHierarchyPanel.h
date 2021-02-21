#pragma once

#include "Chimera/Scene/Scene.h"
#include "Chimera/Core/Log.h"
#include "Chimera/Core/Core.h"
#include "Chimera/Scene/Entity.h"
#include "Chimera/Scene/Components.h"
#include "AssetManagerPanel.h"

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
			//m_Context->m_EditColliders = false;
			if (selectionID >= 0)
			{
				Entity entity = { m_Context->m_Registry.data()[selectionID], m_Context.get() };
				m_SelectionContext = entity;
				//Entity* userData = reinterpret_cast<Entity*>(entity.GetComponent<Body2DComponent>().Body->GetUserData().pointer);
				//CM_CORE_WARN("{0}", (uint32_t)*userData);
			}
			else
			{
				//m_SelectionContext = {};
			}

			 }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;

		fs::path m_CurrentPath;
		fs::path m_RootPath = fs::current_path() / "assets";
		std::vector<ImGuiFilepath::File> m_FilesInScope;
		bool m_IsCurrentPathDir;
		int m_SelectionPath;
	};
}