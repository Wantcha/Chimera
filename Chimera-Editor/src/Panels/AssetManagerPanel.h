#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <filesystem>
#include "../Project/ProjectManager.h"
#include "Chimera/Utils/Utils.h"
//#include <experimental/filesystem>

namespace fs = std::filesystem;

namespace ImGuiFilepath
{
	const struct File
	{
		std::string Alias;
		fs::path Path;
	};

	static const int ClampSize_tToInt(const size_t data)
	{
		constexpr int max_int = std::numeric_limits<int>::max();
		return static_cast<int>(data > max_int ? max_int : data);
	}

	static bool vector_file_items_getter(void* data, int idx, const char** out_text)
	{
		const std::vector<File>* v = reinterpret_cast<std::vector<File>*>(data);
		const int elementCount = ClampSize_tToInt(v->size());
		if (idx < 0 || idx >= elementCount) return false;
		*out_text = v->at(idx).Alias.data();
		return true;
	}

	static std::vector<File> GetFilesInPath(const fs::path& path, bool showParentPath)
	{
		std::vector<File> files;

		if (path.has_parent_path() && showParentPath)
		{
			files.push_back({
				"..",
				path.parent_path()
				});
		}

		for (const fs::directory_entry& dirEntry : fs::directory_iterator(path))
		{
			const fs::path& dirPath = dirEntry.path();
			files.push_back({
				dirPath.filename().string(),
				dirPath
				});
		}
		return files;
	}
}

namespace Chimera
{
	class AssetManagerPanel
	{
	public:
		AssetManagerPanel() = default;
		~AssetManagerPanel() = default;

		void Init();
		void OnImGuiRender();
		void RefreshFiles() { m_FilesInScope = ImGuiFilepath::GetFilesInPath(m_CurrentPath, m_CurrentPath != m_RootPath); }

		const std::string GetOutPath() const { return fs::relative(m_OutPath, ProjectManager::Get().GetProjectPath()).string(); }

	private:
		//const int ClampSize_tToInt(const size_t data);

		int m_Selection;
		fs::path m_CurrentPath;
		fs::path m_CurrentDirectory;
		fs::path m_RootPath/* = fs::current_path()/"assets"*/;
		bool m_CurrentPathIsDir;
		std::string m_OutPath;

		std::string m_TransportedString;

		std::vector<ImGuiFilepath::File> m_FilesInScope;
	};
}