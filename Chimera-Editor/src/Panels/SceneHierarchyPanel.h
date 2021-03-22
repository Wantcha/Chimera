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
		void SetSelectedEntity(Entity entity);
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