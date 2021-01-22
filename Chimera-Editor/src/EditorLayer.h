#pragma once

#include "Chimera.h"
#include "Panels/SceneHierarchyPanel.h"

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
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		OrthographicCameraController m_CameraController;

		//Temp
		//Ref<VertexArray> m_SquareVA;
		//Ref<Shader> m_FlatColorShader;
		//Ref<Texture2D> m_LaurTexture;

		glm::vec4 m_SquareColor{0.3f, 1.0f, 1.0f, 1.0f};

		Ref<Framebuffer> m_Framebuffer;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0.0f, 0.0f};

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		//Panels
		SceneHierarchyPanel m_Panel;
	};
}
