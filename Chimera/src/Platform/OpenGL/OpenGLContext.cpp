#include "cmpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Chimera
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{	
		CM_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}
	void OpenGLContext::Init()
	{
		CM_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CM_CORE_ASSERT(status, "Failed to initialize Glad");

		CM_CORE_INFO("OpenGL Info: ");
		CM_CORE_INFO(" Vendor {0}", glGetString(GL_VENDOR));
		CM_CORE_INFO(" Renderer {0}", glGetString(GL_RENDERER));
		CM_CORE_INFO(" Version {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		CM_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}