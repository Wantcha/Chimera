#include "cmpch.h"
#include "ProjectSerializer.h"
#include "ProjectManager.h"
#include "yaml-cpp/yaml.h"
#include "Chimera/Utils/PlatformUtils.h"
#include "Chimera/Scene/SceneManager.h"
#include <fstream>


namespace Chimera
{
	ProjectSerializer::ProjectSerializer()
	{
	}

	void ProjectSerializer::SerializeEditorSettings()
	{
		std::string filepath = ProjectManager::Get().GetProjectPath() + "\\.EditorSettings";

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Editor User Settings" << YAML::Value << "v1.0";
		out << YAML::Key << "Last Open Scene" << YAML::Value << ProjectManager::Get().GetLastOpenScenePath();

		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool ProjectSerializer::DeserializeEditorSettings()
	{
		std::string editorSettings = ProjectManager::Get().GetProjectPath() + "\\.EditorSettings";
		if (!FileDialogs::FileExists(editorSettings))
			std::ofstream file{ editorSettings };
		std::string filepath = editorSettings;

		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Editor User Settings"])
			return false;

		std::string scenePath = data["Last Open Scene"].as<std::string>();
		ProjectManager::Get().SetLastOpenScenePath(scenePath);

		return true;
	}
	void ProjectSerializer::SerializeBuildSettings()
	{
		std::string filepath = ProjectManager::Get().GetProjectPath() + "\\.BuildSettings";

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Build User Settings" << YAML::Value << "v1.0";
		out << YAML::Key << "Scenes" << YAML::Value << YAML::BeginSeq;

		std::vector<std::string>& scenes = SceneManager::Get().GetScenes();
		for (auto scene : scenes)
		{
			out << YAML::BeginMap;

			out << YAML::Value << scene;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	bool ProjectSerializer::DeserializeBuildSettings()
	{
		std::string buildSettings = ProjectManager::Get().GetProjectPath() + "\\.BuildSettings";
		if (!FileDialogs::FileExists(buildSettings))
			std::ofstream file{ buildSettings };
		std::string filepath = buildSettings;

		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Build User Settings"])
			return false;

		auto scenes = data["Scenes"];
		std::vector<std::string>& sm = SceneManager::Get().GetScenes();

		if (scenes)
		{
			for (auto scene : scenes)
			{
				std::string path = scene.as<std::string>();
				sm.push_back(path);
			}
		}
		return true;
	}
}

