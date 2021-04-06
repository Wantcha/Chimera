#include "cmpch.h"
#include "SceneManager.h"
#include "Chimera/Assets/AssetManager.h"
#include "Chimera/Utils/PlatformUtils.h"
#include "yaml-cpp/yaml.h"

namespace Chimera
{
	SceneManager* SceneManager::s_Instance = nullptr;
	SceneManager::SceneManager()
		: m_ActiveScene(CreateRef<Scene>()), m_ss(m_ActiveScene)
	{
		s_Instance = this;
	}
	void SceneManager::LoadScene(int index)
	{
		NewScene(m_ActiveScene->m_ViewportWidth, m_ActiveScene->m_ViewportHeight);
		m_ss.DeserializeFromFile(AssetManager::GetGameDirectory() + "\\assets" + m_Scenes[index]);
	}
	void SceneManager::SerializeActiveScene(const std::string& filepath)
	{
		m_ss.SerializeToFile(filepath);
	}
	void SceneManager::DeserializeActiveScene(const std::string& filepath)
	{
		m_ss.DeserializeFromFile(filepath);
	}
	void SceneManager::SerializePlayMode()
	{
		m_ss.SerializeToMemory(m_SceneBeforePlayMode);
	}
	void SceneManager::DeserializePlayMode()
	{
		//NewScene(m_ActiveScene->m_ViewportWidth, m_ActiveScene->m_ViewportHeight);
		m_ss.DeserializeFromMemory(m_SceneBeforePlayMode);
	}
	void SceneManager::SerializeEntity(const std::string& filepath, Entity entity)
	{
		m_ss.SerializeEntityWrap(filepath, entity);
	}
	void SceneManager::DeserializeEntity(const std::string& filepath)
	{
		m_ss.DeserializeEntityWrap(filepath);
	}
	void SceneManager::DeserializeSceneList()
	{
		std::string buildSettings = AssetManager::GetGameDirectory() + "\\.BuildSettings";
		if (!FileDialogs::FileExists(buildSettings))
			std::ofstream file{ buildSettings };
		std::string filepath = buildSettings;

		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		auto scenes = data["Scenes"];
		std::vector<std::string>& sm = SceneManager::Get().GetScenes();

		if (scenes)
		{
			CM_CORE_ERROR("YES");
			for (auto scene : scenes)
			{
				std::string path = scene.as<std::string>();
				sm.push_back(path);
			}
		}
	}
}