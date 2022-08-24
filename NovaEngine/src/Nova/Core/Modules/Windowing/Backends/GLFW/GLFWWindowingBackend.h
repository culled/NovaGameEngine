#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Windowing/WindowingBackend.h"

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
		virtual Ref<Window> CreateAndAddWindow(const WindowCreateParams& createParams) override;
		virtual void RemoveWindow(Ref<Window> window) override;

		virtual void SetMainWindow(Ref<Window> window) override;
		virtual Ref<Window> GetMainWindow() const override { return m_MainWindow; }

		virtual void Tick(double deltaTime) override;

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

	private:
		/// True if GLFW has been initialized and can be used
		static bool s_GLFWInitialized;

	private:
		/// <summary>
		/// Callback for when the main window is trying to close
		/// </summary>
		/// <param name="e">The event</param>
		void MainWindowClosingCallback(WindowClosingEvent& e);

	private:
		/// The main window of the application
		Ref<Window> m_MainWindow;

		/// A list of all windows managed by this backend
		List<Ref<Window>> m_Windows;
	};
}