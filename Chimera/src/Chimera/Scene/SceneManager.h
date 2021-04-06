#pragma once
#include "Scene.h"
#include "Entity.h"
#include "SceneSerializer.h"
//#include <filesystem>

namespace Chimera
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager() = default;

		inline static SceneManager& Get()
		{
			if (!s_Instance)
				s_Instance = new SceneManager;
			return *s_Instance;
		}
		std::vector<std::string>& GetScenes() { return m_Scenes; }
		Ref<Scene> GetActiveScene() { return m_ActiveScene; }
		//std::string GetSceneAtIndex(int index) { return m_Scenes[index]; }
		//Ref<Scene> NewScene() { m_ActiveScene = CreateRef<Scene>(); return m_ActiveScene; }

		Ref<Scene> NewScene(uint32_t viewportWidth, uint32_t viewportHeight)
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize(viewportWidth, viewportHeight);
			m_ss = SceneSerializer(m_ActiveScene);
			return m_ActiveScene;
		}

		void LoadScene(int index);

		void SerializeActiveScene(const std::string& filepath);
		void DeserializeActiveScene(const std::string& filepath);

		void SerializePlayMode();
		void DeserializePlayMode();

		void SerializeEntity(const std::string& filepath, Entity entity);
		void DeserializeEntity(const std::string& filepath);

		void DeserializeSceneList();
	private:
		Ref<Scene> m_ActiveScene;
		std::vector<std::string> m_Scenes;
		static SceneManager* s_Instance;
		int m_CurrentIndex = 0;
		SceneSerializer m_ss;
		std::string m_SceneBeforePlayMode;
	};
}
