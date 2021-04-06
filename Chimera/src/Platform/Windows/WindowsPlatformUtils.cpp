#include "cmpch.h"
#include "Chimera/Utils/PlatformUtils.h"

#include <shobjidl_core.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "Chimera/Core/Application.h"
#include <filesystem>

// Works for LINUX and UNIX too
#include <sys/types.h>
#include <sys/stat.h>

namespace Chimera
{
	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	std::string FileDialogs::SelectFolder()
	{
		HRESULT hr = S_OK;

		IFileDialog* pDialog = NULL;
		IShellItem* pItem = NULL;
		DWORD dwFlags;

		LPWSTR pwszFilePath = NULL;

		std::string result;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(
			__uuidof(FileOpenDialog),
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pDialog)
		);
		if (FAILED(hr)) { goto done; }

		hr = pDialog->GetOptions(&dwFlags);

		if (FAILED(hr)) { goto done; }

		hr = pDialog->SetOptions(dwFlags | FOS_PICKFOLDERS);

		if (FAILED(hr)) { goto done; }

		hr = pDialog->SetTitle(L"Select Project Directory");
		if (FAILED(hr)) { goto done; }

		// Show the file-open dialog.
		hr = pDialog->Show(NULL);

		if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
		{
			// User cancelled.
			hr = S_OK;
			goto done;
		}

		if (FAILED(hr)) { goto done; }

		hr = pDialog->GetResult(&pItem);

		if (FAILED(hr)) { goto done; }

		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwszFilePath);

		if (FAILED(hr)) { goto done; }

	done:
		if(pwszFilePath)
			result = wstrtostr(pwszFilePath);

		if (FAILED(hr))
		{
			//ShowErrorMessage(L"Cannot open file.", hr);
			CM_CORE_ERROR("Cannot select folder.");
		}

		CoTaskMemFree(pwszFilePath);
		pDialog->Release();
		pItem->Release();

		return result;
		//SafeRelease(&pItem);
		//SafeRelease(&pDialog);
	}

	bool FileDialogs::IsDirectory(const std::string& path)
	{
		struct stat info;

		if (stat(path.c_str(), &info) != 0)
			return false;
		else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
			return true;

		return false;
	}

	bool FileDialogs::FileExists(const std::string& path)
	{
		std::ifstream infile(path.c_str());
		return infile.good();
	}

	bool FileDialogs::CreateFolder(const std::string& path)
	{
		if (CreateDirectory(stringtowstr(path).c_str(), NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			// CopyFile(...)
			return true;
		}
		else
		{
			// Failed to create directory.
			return false;
		}
	}

	bool FileDialogs::CopyItem(const std::string& from, const std::string& to)
	{
		return CopyFile(stringtowstr(from).c_str(), stringtowstr(to).c_str(), false);
	}

	void FileDialogs::CopyDirectory(const std::string& from, const std::string& to)
	{
		std::filesystem::copy(from, to, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
	}


	std::string FileDialogs::GetExecutableDirectory()
	{
		wchar_t buffer[256];
		GetModuleFileName(NULL, buffer, 256);
		std::string path = wstrtostr(buffer);

		auto lastSlash = path.find_last_of("\\");

		return path.substr(0, lastSlash);
	}

	std::string FileDialogs::wstrtostr(const std::wstring& wstr)
	{
		std::string strTo;
		char* szTo = new char[wstr.length() + 1];
		szTo[wstr.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
		strTo = szTo;
		delete[] szTo;
		return strTo;
	}

	std::wstring FileDialogs::stringtowstr(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
}