#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

/*static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWGGGGGGGGGGWWWWWWWWW"
"WWWWGGGGGGGGGGGGGWWWWWWW"
"WWWGGGGGGGGGGGGGGGWWWWWW"
"WWWGGGGGGGGGGGGGGGGGWWWW"
"WWGGGGGGGGGGGGGGGGGGWWWW"
"WGGGGGGGGGGGGGGGGGGGWWWW"
"WWGGGGGGGGGGGGGGGGGGGWWW"
"WWWGGGGGGGGGGGGGGGGGWWWW"
"WWWWGGGGGGGGGGGGGGGGWWWW"
"WWWWWGGGGGGGGGGGGWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";*/

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f), m_ParticleSystem(10000)
{

}

void Sandbox2D::OnAttach()
{
	/*CM_PROFILE_FUNCTION();
	m_LaurTexture = Chimera::Texture2D::Create("assets/textures/laurgras.png");
	m_SpriteSheet = Chimera::Texture2D::Create("assets/textures/roguelikeCity.png");

	m_BikeTile = Chimera::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 14, 3 }, { 16, 16 }, {1, 1}, 1.0f);
	m_KingTile = Chimera::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 35, 25 }, { 16, 16 }, { 2, 1 }, 1.0f);*/

	/*m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	s_TextureMap['G'] = Chimera::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 1 }, { 16, 16 }, { 1, 1 }, 1.0f);
	s_TextureMap['W'] = Chimera::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 7 }, { 16, 16 }, { 1, 1 }, 1.0f);*/

	/*m_Particle.ColorBegin = { 200 / 255.0f, 100 / 255.0f, 250 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 2.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(5.0f);*/
}

void Sandbox2D::OnDetach()
{
	// CM_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Chimera::Timestep ts)
{
	// CM_PROFILE_FUNCTION();

	////Update
	//m_CameraController.OnUpdate(ts);

	////Render
	//Chimera::Renderer2D::ResetStats();
	//Chimera::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	//Chimera::RenderCommand::Clear();
	//{
	//	static float rotation = 0.0f;
	//	rotation += ts * 30.0f;
	//	CM_PROFILE_SCOPE("Renderer Draw");
	//	Chimera::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//	Chimera::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f, 0.0f }, { 1.3f, 1.5f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });

	//	Chimera::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	//	Chimera::Renderer2D::DrawRotatedQuad({ 0.2f, 0.5f, -0.2f }, { 0.9f, 0.8f }, glm::radians(-rotation / 2), m_LaurTexture, 2.0f, glm::vec4(1.0f, 0.8f, 0.7f, 1.0f));

	//	Chimera::Renderer2D::EndScene();

	//	Chimera::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//	for (float y = -5.0f; y < 5; y += 0.5)
	//	{
	//		for (float x = -5.0f; x < 5; x += 0.5)
	//		{
	//			glm::vec4 color = { (x + 5) / 10.0f, 0.4f, (y + 5) / 10.0f, 0.6f };
	//			Chimera::Renderer2D::DrawQuad({ x, y, -0.9f }, { 0.45f, 0.45f }, color);
	//		}
	//	}
	//	Chimera::Renderer2D::EndScene();
	//}
	//if (Chimera::Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	//{
	//	auto [x, y] = Chimera::Input::GetMousePosition();
	//	auto width = Chimera::Application::Get().GetWindow().GetWidth();
	//	auto height = Chimera::Application::Get().GetWindow().GetHeight();

	//	auto bounds = m_CameraController.GetBounds();
	//	auto pos = m_CameraController.GetCamera().GetPosition();
	//	x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
	//	y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
	//	m_Particle.Position = { x + pos.x, y + pos.y };
	//	for (int i = 0; i < 20; i++)
	//	{
	//		m_ParticleSystem.Emit(m_Particle);
	//	}
	//}

	//m_ParticleSystem.OnUpdate(ts);
	//m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	//Chimera::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//for (uint32_t y = 0; y < m_MapHeight; y++)
	//{
	//	for (uint32_t x = 0; x < m_MapWidth; x++)
	//	{
	//		char tileType = s_MapTiles[x + y * m_MapWidth];
	//		Chimera::Ref<Chimera::SubTexture2D> texture;
	//		if (s_TextureMap.find(tileType) != s_TextureMap.end())
	//		{
	//			texture = s_TextureMap[tileType];
	//		}
	//		else
	//			texture = m_KingTile;

	//		Chimera::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f ,m_MapHeight - y - m_MapHeight / 2.0f, -0.1f }, { 1.0f, 1.0f }, texture);
	//	}
	//}

	//Chimera::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_GrassTile);
	//Chimera::Renderer2D::DrawQuad({ 2.0f, 0.0f, 0.0f }, { 2.0f, 1.0f }, m_KingTile);
	//Chimera::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	/*CM_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Chimera::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();*/
}

void Sandbox2D::OnEvent(Chimera::Event& e)
{
	//m_CameraController.OnEvent(e);
}
