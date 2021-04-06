#pragma once

#include <string>
#include "ProjectSerializer.h"
#include "Chimera/Assets/AssetManager.h"

namespace Chimera
{
	class ProjectManager
	{
	public:
		ProjectManager();
		~ProjectManager() = default;

		//void SaveEditorSettings();
		void SetProjectDirectories();
		inline static ProjectManager& Get()
		{
			if (!s_Instance)
				s_Instance = new ProjectManager();
			return *s_Instance;
		}

		// TODO: Make only one project/game path. This is not ideal!!
		void SetProjectPath(const std::string& path) { m_ProjectPath.assign(path); AssetManager::SetGameDirectory(path); }
		void SetLastOpenScenePath(const std::string& path) { m_LastOpenScenePath = path;}

		std::string GetProjectPath() const { return m_ProjectPath; }
		std::string GetLastOpenScenePath() const { return m_LastOpenScenePath; }

	private:
		std::string m_ProjectPath = "";
		std::string m_LastOpenScenePath = "";

		ProjectSerializer m_Serializer;
		static ProjectManager* s_Instance;
	};
}
