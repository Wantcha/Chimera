#pragma once

#include <string>

namespace Chimera
{
	class FileDialogs
	{
	public:
		// These returns empty string if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		static std::string SelectFolder();
		static bool IsDirectory(const std::string& path);
		static bool FileExists(const std::string& path);
		static bool CreateFolder(const std::string& path);
		static bool CopyItem(const std::string& from, const std::string& to);
		static void CopyDirectory(const std::string& from, const std::string& to);
		static std::string GetExecutableDirectory();

	private:
		static std::string wstrtostr(const std::wstring& wstr);
		static std::wstring stringtowstr(const std::string& s);
	};
}
