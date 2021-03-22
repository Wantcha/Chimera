#pragma once

#include "Chimera.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/AssetManagerPanel.h"

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

		OrthographicCameraController m_CameraController;

		//Temp
		//Ref<VertexArray> m_SquareVA;
		//Ref<Shader> m_FlatColorShader;
		//Ref<Texture2D> m_LaurTexture;

		glm::vec4 m_SquareColor{0.3f, 1.0f, 1.0f, 1.0f};

		Ref<Framebuffer> m_Framebuffer;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		glm::vec2 m_ViewportBounds[2];

		Entity m_HoveredEntity;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;
		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;
		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		AssetManagerPanel m_AssetManagerPanel;
	};
}
