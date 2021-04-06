#include "cmpch.h"
#include "SceneManagerPanel.h"
#include "Chimera/Scene/SceneManager.h"
#include "Chimera/Utils/Utils.h"
#include "imgui/imgui.h"
#include "../Project/ProjectSerializer.h"

namespace Chimera
{
	void SceneManagerPanel::OnImGuiRender()
	{
		if (m_ShowSceneManagerWindow)
		{
			if (ImGui::Begin("Scene Manager", &m_ShowSceneManagerWindow))
			{
				ImGui::PushItemWidth(-1);

				std::vector<std::string>& scenes = SceneManager::Get().GetScenes();
				ImGui::BeginListBox("Scenes");

				int i = 0;
				for (std::string scene : scenes)
				{
					if (ImGui::Selectable(scene.c_str(), m_CurrentSelection == i))
					{
						m_CurrentSelection = i;
						m_CurrentSelectionPath = scene;

						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
						{
							// Set payload to carry the index of our item (could be anything)
							ImGui::SetDragDropPayload("Dragged_Scene", &m_CurrentSelection, sizeof(int));
							CM_CORE_ERROR(m_CurrentSelection);
							ImGui::EndDragDropSource();
						}
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Scene"))
							{
								IM_ASSERT(payload->DataSize == sizeof(int));
								int payload_n = *(const int*)payload->Data;

								std::iter_swap(scenes.begin() + payload_n, scenes.begin() + m_CurrentSelection);
							}
							ImGui::EndDragDropTarget();
						}
					}
					++i;
				}

				ImGui::EndListBox();

				ImGui::PopItemWidth();

				if (ImGui::Button("Add Active Scene"))
				{
					if (m_ActiveScenePath != "")
					{
						std::vector<std::string>& sceneList = SceneManager::Get().GetScenes();
						if (!VectorUtils::IsElementInVector<std::string>(sceneList, m_ActiveScenePath))
						{
							sceneList.push_back(m_ActiveScenePath);
						}
						ProjectSerializer ps;
						ps.SerializeBuildSettings();
					}

				}
				ImGui::SameLine();
				static const ImVec4 disabledColor = { 0.3f, 0.3f, 0.3f, 1.0f };

				if (m_CurrentSelectionPath == "")
				{
					ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);

					ImGui::Button("Remove Scene");

					ImGui::PopStyleColor();
					ImGui::PopStyleColor();
					ImGui::PopStyleColor();
				}

				else
				{
					if (ImGui::Button("Remove Scene"))
					{
						std::vector<std::string>::const_iterator pos = VectorUtils::GetElementPosition<std::string>(scenes, m_CurrentSelectionPath);
						scenes.erase(pos);
						m_CurrentSelectionPath = "";
						ImGui::CloseCurrentPopup();
						ProjectSerializer ps;
						ps.SerializeBuildSettings();
					}
				}

				ImGui::End();
			}
		}
	}
}