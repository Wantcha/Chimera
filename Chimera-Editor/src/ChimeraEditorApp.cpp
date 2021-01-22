#include <Chimera.h>

//--------ENTRY POINT----------------
#include <Chimera/Core/EntryPoint.h>
//-----------------------------------

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"

#include "EditorLayer.h"

namespace Chimera
{
	class ChimeraEditor : public Application
	{
	public:
		ChimeraEditor()
			: Application("Chimera Editor")
		{
			PushLayer(new EditorLayer());
		}

		~ChimeraEditor()
		{

		}
	};

	Application* Chimera::CreateApplication()
	{
		return new ChimeraEditor();
	}
}