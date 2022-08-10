#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/AppModule.h"
#include "Window.h"

namespace Nova::Display
{
	NovaClass DisplayModule : public AppModule
	{
	public:
		static Ref<DisplayModule> Get();

	public:
		DisplayModule(int tickOffset);

		virtual ~DisplayModule();

		virtual void Init() override;

		/// <summary>
		/// Creates a window and adds it to this display module's list of windows
		/// </summary>
		/// <param name="width">The initial width of the window</param>
		/// <param name="height">The initial height of the window</param>
		/// <param name="title">The title of the window</param>
		/// <returns>The created window</returns>
		Ref<Window> CreateAndAddWindow(uint32_t width, uint32_t height, const string& title);

		/// <summary>
		/// Gets the number of windows in this display module
		/// </summary>
		/// <returns>The number of windows in this display module</returns>
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

	protected:
		virtual int GetDefaultTickOrder() const  override { return -1000; }

		virtual Ref<Window> CreateWindow(uint32_t width, uint32_t height, const string& title) = 0;

		void MainWindowClosedCallback(Ref<Event> e);

	protected:
		static WeakRef<DisplayModule> s_Instance;

		Ref<Window> m_MainWindow;
		List<Ref<Window>> m_Windows;
	};
}