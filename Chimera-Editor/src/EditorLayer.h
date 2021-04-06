#pragma once

#include "Chimera.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/AssetManagerPanel.h"
#include "Panels/SceneManagerPanel.h"

#include "Chimera/Renderer/EditorCamera.h"


namespace Chimera
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnFixedUpdate(float fixedts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		void SetProject();
		void OpenProject(const std::string& path = "");
		void BuildProject();

		void RestoreSceneFromPlayMode();

		Ref<Framebuffer> m_ViewportFramebuffer, m_GameWindowFramebuffer;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		glm::vec2 m_ViewportBounds[2];

		glm::vec2 m_GameWindowSize = { 0.0f, 0.0f };
		glm::vec2 m_GameWindowBounds[2];

		Entity m_HoveredEntity;

		// bool m_PrimaryCamera = true;
		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;
		bool m_IsPlayMode = false;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		AssetManagerPanel m_AssetManagerPanel;
		SceneManagerPanel m_SceneManagerPanel;

		bool m_ShowProjectWindow = true;
	};
}
