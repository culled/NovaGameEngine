#include "GLFWMonitor.h"

namespace Nova::Windowing
{
	GLFWMonitor::GLFWMonitor(GLFWmonitor* internalMonitor) :
		m_InternalMonitor(internalMonitor)
	{}

	// Window ----------
	string GLFWMonitor::GetName() const
	{
		return string(glfwGetMonitorName(m_InternalMonitor));
	}

	Vector2i GLFWMonitor::GetPosition() const
	{
		Vector2i pos;
		glfwGetMonitorPos(m_InternalMonitor, &pos.X, &pos.Y);
		return pos;
	}

	Vector2i GLFWMonitor::GetPhysicalSize() const
	{
		Vector2i size;
		glfwGetMonitorPhysicalSize(m_InternalMonitor, &size.X, &size.Y);
		return size;
	}

	Recti GLFWMonitor::GetWorkArea() const
	{
		Recti workArea;
		glfwGetMonitorWorkarea(m_InternalMonitor, &workArea.Position.X, &workArea.Position.Y, &workArea.Size.X, &workArea.Size.Y);
		return workArea;
	}

	List<MonitorVideoMode> GLFWMonitor::GetSupportedVideoModes() const
	{
		int count = 0;
		const GLFWvidmode* glfwVidModes = glfwGetVideoModes(m_InternalMonitor, &count);

		List<MonitorVideoMode> videoModes;
		for (int i = 0; i < count; i++)
		{
			videoModes.push_back(ConvertGlfwVidmode(&glfwVidModes[i]));
		}

		return videoModes;
	}

	MonitorVideoMode GLFWMonitor::GetCurrentVideoMode() const
	{
		const GLFWvidmode* currentVidMode = glfwGetVideoMode(m_InternalMonitor);
		return ConvertGlfwVidmode(currentVidMode);
	}

	bool GLFWMonitor::IsPrimary() const
	{
		return glfwGetPrimaryMonitor() == m_InternalMonitor;
	}

	Vector2 GLFWMonitor::GetDpiScale() const
	{
		float x, y;
		glfwGetMonitorContentScale(m_InternalMonitor, &x, &y);
		return Vector2((double)x, (double)y);
	}

	// Window ----------

	MonitorVideoMode GLFWMonitor::ConvertGlfwVidmode(const GLFWvidmode* glfwVideomode)
	{
		return MonitorVideoMode(Vector2i(glfwVideomode->width, glfwVideomode->height), 
			glfwVideomode->redBits, glfwVideomode->greenBits, glfwVideomode->blueBits, glfwVideomode->refreshRate);
	}
}