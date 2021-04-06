#pragma once

#include "Chimera/Scene/SceneManager.h"
#include "Chimera/Scene/Scene.h"

namespace Chimera
{
	class SceneManagerPanel
	{
	public:
		SceneManagerPanel() = default;
		~SceneManagerPanel() = default;

		void OnImGuiRender();
		void SetPanelVisiblity(bool visibility) { m_ShowSceneManagerWindow = visibility; }

		void SetActiveScenePath(const std::string& path) { m_ActiveScenePath = path; }
		std::string GetActiveScene() const { return m_ActiveScenePath; }

	private:
		bool m_ShowSceneManagerWindow = false;
		int m_CurrentSelection;
		std::string m_CurrentSelectionPath = "";

		std::string m_ActiveScenePath = "";
	};
}
