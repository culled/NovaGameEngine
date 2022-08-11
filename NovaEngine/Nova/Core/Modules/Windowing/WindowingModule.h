#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Modules/AppModule.h"
#include "Window.h"

namespace Nova::Windowing
{
	/// <summary>
	/// AppModule that handles graphical windows on a user's desktop
	/// </summary>
	NovaClass WindowingModule : public AppModule
	{
	public:
		WindowingModule(int tickOffset);

		virtual ~WindowingModule();

	// RefCounted ----------
	public:
		virtual void Init() override;

	// RefCounted ----------

	// AppModule ----------
	protected:
		virtual int GetDefaultTickOrder() const override { return -1000; }

	// AppModule ----------

	public:
		/// <summary>
		/// Gets the active WindowingModule singleton
		/// </summary>
		/// <returns>A reference to the active WindowingModule singleton</returns>
		static Ref<WindowingModule> Get();

	protected:
		// The current WindowingModule singleton
		static WeakRef<WindowingModule> s_Instance;

	public:
		/// <summary>
		/// Creates a window and adds it to this windowing module's list of windows
		/// </summary>
		/// <param name="width">The initial width of the window</param>
		/// <param name="height">The initial height of the window</param>
		/// <param name="title">The title of the window</param>
		/// <returns>The created window</returns>
		Ref<Window> CreateAndAddWindow(const WindowCreateParams& createParams);

		/// <summary>
		/// Gets the number of windows in this windowing module
		/// </summary>
		/// <returns>The number of windows in this windowing module</returns>
		size_t GetWindowCount() const { return m_Windows.size(); }

		/// <summary>
		/// Sets the given window as the "main window" for this application
		/// </summary>
		/// <param name="window">The window to set as the main window</param>
		void SetMainWindow(Ref<Window> window);

		/// <summary>
		/// Gets the main window for this application (if one has been assigned/created)
		/// </summary>
		/// <returns>The main window, or nullptr if no window has been created/assigned</returns>
		Ref<Window> GetMainWindow() const { return m_MainWindow; }

		/// <summary>
		/// Called when a window closed
		/// </summary>
		/// <param name="window">The window that closed</param>
		void WindowClosed(Ref<Window> window);

	protected:
		/// <summary>
		/// Virtual method for creating a window
		/// </summary>
		/// <returns>A created window</returns>
		virtual Ref<Window> CreateWindow(const WindowCreateParams& createParams) = 0;

		virtual void ReleaseAllWindows();

		/// <summary>
		/// Callback for when the main window is trying to close
		/// </summary>
		/// <param name="e">The event</param>
		void MainWindowClosingCallback(WindowClosingEvent& e);

	protected:
		// The main window of the application
		Ref<Window> m_MainWindow;

		// A list of all windows managed by this WindowingModule
		List<Ref<Window>> m_Windows;
	};
}