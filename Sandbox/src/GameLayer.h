#pragma once

#include "Chimera.h"

class GameLayer : public Chimera::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Chimera::Timestep ts) override;
	virtual void OnFixedUpdate(float fixedts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Chimera::Event& e) override;
private:
	bool OnWindowResize(Chimera::WindowResizeEvent& e);

	//Chimera::Ref<Chimera::Scene> m_ActiveScene;
	Chimera::Ref<Chimera::Framebuffer> m_Framebuffer;
};