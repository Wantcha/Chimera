#pragma once

#include "Chimera/Core/Layer.h"
#include "Chimera/Events/KeyEvent.h"
#include "Chimera/Events/MouseEvent.h"
#include "Chimera/Events/ApplicationEvent.h"

namespace Chimera
{
	class CHIMERA_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}
