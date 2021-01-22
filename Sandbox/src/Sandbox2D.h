#pragma once

#include "Chimera.h"

#include "ParticleSystem.h"

class Sandbox2D: public Chimera::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Chimera::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Chimera::Event& e) override;
private:
	Chimera::OrthographicCameraController m_CameraController;

	//Temp
	Chimera::Ref<Chimera::VertexArray> m_SquareVA;
	Chimera::Ref<Chimera::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	Chimera::Ref<Chimera::Texture2D> m_LaurTexture;
	Chimera::Ref<Chimera::Texture2D> m_SpriteSheet;
	Chimera::Ref<Chimera::SubTexture2D> m_BikeTile, m_KingTile, m_GrassTile, m_WallTile;

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;

	/*uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Chimera::Ref<Chimera::SubTexture2D>> s_TextureMap;*/
};
