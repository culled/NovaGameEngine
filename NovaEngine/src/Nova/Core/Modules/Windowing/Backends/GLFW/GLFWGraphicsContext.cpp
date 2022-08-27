#include "GLFWGraphicsContext.h"

#include "Nova/Core/Modules/Rendering/RenderingBackend.h"
#include "Nova/Core/Modules/Rendering/RenderModule.h"

#include <GLFW/glfw3.h>

namespace Nova::Windowing
{
	GLFWGraphicsContext::GLFWGraphicsContext(GLFWwindow* internalContext, bool isShared) :
		m_InternalContext(internalContext), m_IsShared(isShared)
	{}

	//RefCounted ----------
	void GLFWGraphicsContext::Init()
	{
		Rendering::RenderModule::Get()->GetBackend()->InitGraphicsContext(GetSelfRef<GLFWGraphicsContext>());
	}

	//RefCounted ----------

	void GLFWGraphicsContext::MakeCurrent()
	{
		EnsureInternalContext();

		glfwMakeContextCurrent(m_InternalContext);
	}

	void GLFWGraphicsContext::ReleaseCurrent()
	{
		EnsureInternalContext();

		glfwMakeContextCurrent(nullptr);
	}

	void* GLFWGraphicsContext::GetExtensionsFunction() const
	{
		return glfwGetProcAddress;
	}

	void GLFWGraphicsContext::SwapBuffers()
	{
		EnsureInternalContext();

		glfwSwapBuffers(m_InternalContext);
	}

	Vector2i GLFWGraphicsContext::GetFramebufferSize() const
	{
		EnsureInternalContext();

		Vector2i size;
		glfwGetFramebufferSize(m_InternalContext, &size.X, &size.Y);
		return size;
	}

	void GLFWGraphicsContext::EnsureInternalContext() const
	{
		if (!m_InternalContext)
			throw Exception("No internal context has been set!");
	}
}