#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Modules/AppModule.h"
#include "Window.h"
#include "WindowingBackend.h"

namespace Nova::Windowing
{
	/// <summary>
	/// AppModule that handles graphical windows on a user's desktop
	/// </summary>
	class NovaAPI WindowingModule : public AppModule
	{
	public:
		WindowingModule(int tickOffset, WindowingAPI windowingAPI);
		~WindowingModule();

	// RefCounted ----------
	protected:
		virtual void Init() override;

	// RefCounted ----------

	// TickListener ----------
	public:
		virtual void Tick(double deltaTime) override;

	// TickListener ----------
	
	// AppModule ----------
	protected:
		virtual int GetDefaultTickOrder() const override { return static_cast<int>(AppModule::BuiltInModuleOffset::WindowingModule); }

	// AppModule ----------

	public:
		/// <summary>
		/// Gets the active WindowingModule singleton
		/// </summary>
		/// <returns>A reference to the active WindowingModule singleton</returns>
		static Ref<WindowingModule> Get() { return s_Instance.lock(); }

	protected:
		// The current WindowingModule singleton
		static WeakRef<WindowingModule> s_Instance;

	public:
		/// <summary>
		/// Gets the backend for this WindowingModule
		/// </summary>
		/// <returns>The backend for this WindowingModule</returns>
		Ref<WindowingBackend> GetBackend() const { return m_WindowingBackend; }

		/// <summary>
		/// Creates a window and adds it to this windowing module's list of windows
		/// </summary>
		/// <param name="width">The initial width of the window</param>
		/// <param name="height">The initial height of the window</param>
		/// <param name="title">The title of the window</param>
		/// <returns>The created window</returns>
		Ref<Window> CreateAndAddWindow(const WindowCreateParams& createParams);

		/// <summary>
		/// Sets the given window as the "main window" for this application
		/// </summary>
		/// <param name="window">The window to set as the main window</param>
		void SetMainWindow(Ref<Window> window) { m_WindowingBackend->SetMainWindow(window); }

		/// <summary>
		/// Gets the main window for this application (if one has been assigned/created)
		/// </summary>
		/// <returns>The main window, or nullptr if no window has been created/assigned</returns>
		Ref<Window> GetMainWindow() const { return m_WindowingBackend->GetMainWindow(); }

	private:
		/// <summary>
		/// Creates the windowing backend
		/// </summary>
		/// <param name="backendAPI">The backend API to create</param>
		void CreateBackend(WindowingAPI backendAPI);

	private:
		/// <summary>
		/// The backend this WindowingModule uses
		/// </summary>
		Ref<WindowingBackend> m_WindowingBackend;
	};
}