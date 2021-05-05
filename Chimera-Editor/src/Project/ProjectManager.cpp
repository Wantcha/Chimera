#include "cmpch.h"
#include "ProjectManager.h"
#include "Chimera/Utils/PlatformUtils.h"

namespace Chimera
{
	ProjectManager* ProjectManager::s_Instance = nullptr;

	ProjectManager::ProjectManager()
	{
		s_Instance = this;
	}

	void ProjectManager::SetProjectDirectories()
	{
		//CM_CORE_ERROR(m_ProjectPath);
		std::string assets = m_ProjectPath + "\\assets";
		std::string library = m_ProjectPath + "\\library";
		FileDialogs::CreateFolder(assets);
		FileDialogs::CreateFolder(library);

		FileDialogs::CopyItem("Library/Texture.glsl", m_ProjectPath + "\\library\\Texture.glsl");

		if (!FileDialogs::FileExists(m_ProjectPath + "\\.EditorSettings"))
			std::ofstream file{ m_ProjectPath + "\\.EditorSettings" };

		//FileDialogs::CreateFolder(m_ProjectPath + "/assets");
	}

}