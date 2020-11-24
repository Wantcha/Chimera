#pragma once

#include "Chimera/Layer.h"
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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
