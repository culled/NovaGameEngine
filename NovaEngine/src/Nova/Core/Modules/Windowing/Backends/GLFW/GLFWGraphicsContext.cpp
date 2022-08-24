#include "GLFWGraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Nova::Windowing
{
	GLFWGraphicsContext::GLFWGraphicsContext(uint32_t width, uint32_t height) :
		m_Width(width), m_Height(height)
	{}

	void GLFWGraphicsContext::MakeCurrent()
	{
		if (!m_InternalContext)
			throw Exception("No internal context has been set!");

		glfwMakeContextCurrent(m_InternalContext);
	}

	void* GLFWGraphicsContext::GetExtensionsFunction() const
	{
		return glfwGetProcAddress;
	}

	void GLFWGraphicsContext::SwapBuffers()
	{
		if (!m_InternalContext)
			throw Exception("No internal context has been set!");

		glfwSwapBuffers(m_InternalContext);
	}

}