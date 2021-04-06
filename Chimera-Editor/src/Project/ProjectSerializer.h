#pragma once

#include <string>

namespace Chimera
{
	class ProjectSerializer
	{
	public:
		ProjectSerializer();
		~ProjectSerializer() = default;

		void SerializeEditorSettings();
		bool DeserializeEditorSettings();

		void SerializeBuildSettings();
		bool DeserializeBuildSettings();
	};
}
