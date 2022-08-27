#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Windowing/Monitor.h"

#include <GLFW/glfw3.h>

namespace Nova::Windowing
{
	/// <summary>
	/// GLFW implmentation of a monitor
	/// </summary>
	class NovaAPI GLFWMonitor : public Monitor
	{
	public:
		GLFWMonitor(GLFWmonitor* internalMonitor);
		~GLFWMonitor() = default;

		// Monitor ----------
	public:
		virtual string GetName() const override;
		virtual Vector2i GetPosition() const override;
		virtual Vector2i GetPhysicalSize() const override;
		virtual Recti GetWorkArea() const override;
		virtual List<MonitorVideoMode> GetSupportedVideoModes() const override;
		virtual MonitorVideoMode GetCurrentVideoMode() const override;
		virtual bool IsPrimary() const override;
		virtual Vector2 GetDpiScale() const override;

		// Monitor ----------

		/// <summary>
		/// Gets the internal monitor
		/// </summary>
		/// <returns>The internal monitor</returns>
		GLFWmonitor* GetInternalMonitor() const { return m_InternalMonitor; }

	private:
		/// <summary>
		/// Converts a GLFWvidmode to a MonitorVideoMode
		/// </summary>
		/// <param name="glfwVideomode">The GLFWvidmode</param>
		/// <returns>The MonitorVideoMode representation of the GLFWvidmode</returns>
		static MonitorVideoMode ConvertGlfwVidmode(const GLFWvidmode* glfwVideomode);

	private:
		/// <summary>
		/// The internal GLFWmonitor pointer
		/// </summary>
		GLFWmonitor* m_InternalMonitor;
	};
}