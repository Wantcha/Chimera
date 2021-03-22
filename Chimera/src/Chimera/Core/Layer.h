#pragma once

#include "Chimera/Core/Core.h"
#include "Chimera/Core/Timestep.h"
#include "Chimera/Events/Event.h"

namespace Chimera
{
	class CHIMERA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnFixedUpdate(float fixedts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}