#include "GameLayer.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "Chimera/Scene/SceneManager.h"
#include "Chimera/Utils/PlatformUtils.h"
#include "Chimera/Assets/AssetManager.h"

#include "Chimera/Math/Math.h"


GameLayer::GameLayer()
	: Layer("GameLayer")
{

}

void GameLayer::OnAttach()
{
	/*Chimera::FramebufferSpecification fbSpec;
	fbSpec.Attachments = { Chimera::FramebufferTextureFormat::RGBA8, Chimera::FramebufferTextureFormat::RED_INTEGER, Chimera::FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Chimera::Framebuffer::Create(fbSpec);*/
	//m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	//m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	Chimera::SceneManager::Get().NewScene(1280, 720);
	//Chimera::SceneSerializer serializer(m_ActiveScene);
	//m_ActiveScene->OnViewportResize(1280, 720);
	//CM_CORE_ERROR(Chimera::AssetManager::GetGameDirectory() + "\\assets" + Chimera::SceneManager::Get().GetScenes()[0]);
	Chimera::SceneManager::Get().DeserializeActiveScene(Chimera::AssetManager::GetGameDirectory() + "\\assets" + Chimera::SceneManager::Get().GetScenes()[0]);
	//serializer.Deserialize(Chimera::AssetManager::GetGameDirectory()+ "\\assets" + Chimera::SceneManager::Get().GetScenes()[0]);

}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Chimera::Timestep ts)
{
	//if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
	//	m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
	//	(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	//{
	//	m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	//	//m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
	//	m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	//	m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	//}

	Chimera::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Chimera::RenderCommand::Clear();
	Chimera::SceneManager::Get().GetActiveScene()->OnUpdateRuntime(ts);
	Chimera::SceneManager::Get().GetActiveScene()->OnRenderRuntime();
}

void GameLayer::OnFixedUpdate(float fixedts)
{
	Chimera::SceneManager::Get().GetActiveScene()->OnFixedUpdate(fixedts);
}

void GameLayer::OnImGuiRender()
{

}

void GameLayer::OnEvent(Chimera::Event& e)
{
	Chimera::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Chimera::WindowResizeEvent>(std::bind(&GameLayer::OnWindowResize, this, std::placeholders::_1));
}

bool GameLayer::OnWindowResize(Chimera::WindowResizeEvent& e)
{
	Chimera::SceneManager::Get().GetActiveScene()->OnViewportResize(e.GetWidth(), e.GetHeight());
	return false;
}
