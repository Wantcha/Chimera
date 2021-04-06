#include "AssetManagerPanel.h"
#include "Chimera/Scene/Entity.h"
#include "Chimera/Scene/SceneManager.h"
#include <imgui/imgui.h>


namespace Chimera
{
	void AssetManagerPanel::Init()
	{
		m_CurrentPath = ProjectManager::Get().GetProjectPath();
		m_CurrentPath /= "assets";
		m_RootPath = m_CurrentPath;
		m_CurrentDirectory = m_CurrentPath;
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

		if (ImGui::BeginListBox("##", ImVec2(0, -1)))
		{
			for (ImGuiFilepath::File& file : m_FilesInScope)
			{
				std::filesystem::path path = file.Path;

				m_CurrentPathIsDir = fs::is_directory(path);
				if (ImGui::Selectable(file.Alias.c_str()))
				{
					m_CurrentPath = path;
					if (m_CurrentPathIsDir) {
						m_CurrentDirectory = m_CurrentPath;
						m_FilesInScope = ImGuiFilepath::GetFilesInPath(m_CurrentPath, m_CurrentPath != m_RootPath);
						break;
					}
					
				}
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Set payload to carry the index of our item (could be anything)
					ImGui::SetDragDropPayload("Dragged_File", &file.Path, sizeof(std::filesystem::path));
					ImGui::Text(file.Path.string().c_str());
					ImGui::EndDragDropSource();
				}
				
			}
			ImGui::EndListBox();
		}
		ImGui::PopItemWidth();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Entity"))
			{
				IM_ASSERT(payload->DataSize == sizeof(Entity));
				Entity payload_n = *(const Entity*)payload->Data;

				SceneManager::Get().SerializeEntity(m_CurrentDirectory.string(), payload_n);
				m_FilesInScope = ImGuiFilepath::GetFilesInPath(m_CurrentPath, m_CurrentPath != m_RootPath);

			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
	}
}