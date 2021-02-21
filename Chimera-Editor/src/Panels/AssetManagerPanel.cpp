#include "AssetManagerPanel.h"
#include <imgui/imgui.h>


namespace Chimera
{
	void AssetManagerPanel::Init()
	{
		m_CurrentPath = fs::current_path();
		m_CurrentPath /= "assets";
		m_CurrentPathIsDir = true;

		m_FilesInScope = ImGuiFilepath::GetFilesInPath(m_CurrentPath, m_CurrentPath != m_RootPath);
	}

	void AssetManagerPanel::OnImGuiRender()
	{
		ImGui::Begin("Project Manager");

		//Auto resize text wrap to popup width.
		ImGui::Spacing();
		ImGui::PushItemWidth(-1);
		ImGui::TextWrapped(m_CurrentPath.string().data());
		ImGui::PopItemWidth();

		ImVec2 size = ImGui::GetContentRegionAvail();

		ImGui::SameLine(size.x - 50);

		// Make the "Select" button look / act disabled if the current selection is a directory.
		if (m_CurrentPathIsDir)
		{
			static const ImVec4 disabledColor = { 0.3f, 0.3f, 0.3f, 1.0f };

			ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);

			ImGui::Button("Select");

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

		}
		else
		{
			if (ImGui::Button("Select"))
			{
				ImGui::CloseCurrentPopup();

				m_OutPath = m_CurrentPath.string();
			}
		}

		ImGui::PushItemWidth(-1);
		if (ImGui::ListBox("##", &m_Selection, ImGuiFilepath::vector_file_items_getter, &m_FilesInScope, m_FilesInScope.size(),
			size.y / (ImGui::GetStyle().ItemSpacing.y + ImGui::GetFontSize()) - 1))
		{
			//Update current path to the selected list item.
			m_CurrentPath = m_FilesInScope[m_Selection].Path;
			m_CurrentPathIsDir = fs::is_directory(m_CurrentPath);

			//If the selection is a directory, repopulate the list with the contents of that directory.
			if (m_CurrentPathIsDir) {
				m_FilesInScope = ImGuiFilepath::GetFilesInPath(m_CurrentPath, m_CurrentPath != m_RootPath);
			}

		}
		ImGui::PopItemWidth();

		ImGui::End();
	}
}