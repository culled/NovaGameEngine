#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Windowing/WindowingBackend.h"
#include "Nova/Core/Modules/Windowing/Monitor.h"

// Forward declare so we don't include glfw here
struct GLFWmonitor;

namespace Nova::Windowing
{
	/// <summary>
	/// A windowing backend that uses GLFW
	/// </summary>
	class NovaAPI GLFWWindowingBackend : public WindowingBackend
	{
	public:
		GLFWWindowingBackend();
		~GLFWWindowingBackend();

	// WindowingBackend ----------
	public:
		virtual WindowingAPI GetAPI() const override { return WindowingAPI::GLFW; }
		virtual Ref<Window> CreateWindow(const WindowCreateParams& createParams) override;

		virtual void Tick(double deltaTime) override;
		virtual List<Ref<Monitor>> GetMonitors() const override { return m_Monitors; }

	// WindowingBackend ----------

	private:
		/// <summary>
		/// Initializes GLFW
		/// </summary>
		static void InitGLFW();

		/// <summary>
		/// Terminates GLFW
		/// </summary>
		static void TerminateGLFW();

		/// <summary>
		/// Callback for when a monitor event happens in glfw
		/// </summary>
		/// <param name="monitor">The monitor</param>
		/// <param name="event">The event that happened</param>
		static void MonitorCallback(GLFWmonitor* monitor, int eventType);

	private:
		/// <summary>
		/// True if GLFW has been initialized and can be used
		/// </summary>
		static bool s_GLFWInitialized;

	private:
		/// <summary>
		/// Callback for when the main window is trying to close
		/// </summary>
		/// <param name="e">The event</param>
		void MainWindowClosingCallback(WindowClosingEvent& e);

		/// <summary>
		/// Initializes monitors
		/// </summary>
		void InitializeMonitors();

		/// <summary>
		/// Called when a monitor is connected to the system
		/// </summary>
		/// <param name="monitor">The monitor that connected</param>
		void MonitorConnected(GLFWmonitor* monitor);

		/// <summary>
		/// Called when a monitor is disconnected from the system
		/// </summary>
		/// <param name="monitor">The monitor that disconnected</param>
		void MonitorDisconnected(GLFWmonitor* monitor);

	private:
		/// <summary>
		/// A list of monitors connected to this system
		/// </summary>
		List<Ref<Monitor>> m_Monitors;
	};
}