#pragma once

#include "Scene.h"
#include <yaml-cpp/yaml.h>
#include <sstream>

namespace Chimera
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeToFile(const std::string& filepath);
		void SerializeToMemory(std::string& buffer);
		void SerializeEntityWrap(const std::string& filepath, Entity entity);

		bool DeserializeFromFile(const std::string& filepath);
		bool DeserializeFromMemory(const std::string& buffer);
		bool DeserializeEntityWrap(const std::string& filepath);
	private:
		std::string Serialize();
		bool Deserialize(const std::string& buffer);
		bool DeserializeEntities(YAML::Node& node);

		Ref<Scene> m_Scene;
	};
}