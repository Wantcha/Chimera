#include "cmpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Chimera
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}