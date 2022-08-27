#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Modules/AppModule.h"
#include "Window.h"
#include "WindowingBackend.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Events/EventSource.h"

namespace Nova::Windowing
{
	/// <summary>
	/// Event for when a window is created
	/// </summary>
	struct NovaAPI WindowCreatedEvent : public Event
	{
		WindowCreatedEvent(const Ref<Window>& window);
		~WindowCreatedEvent() = default;

		/// <summary>
		/// The window that was created
		/// </summary>
		Ref<Window> Window;
	};

	/// <summary>
	/// Event for when a window is destroyed
	/// </summary>
	struct NovaAPI WindowDestroyedEvent : public Event
	{
		WindowDestroyedEvent(const Ref<Window>& window);
		~WindowDestroyedEvent() = default;

		/// <summary>
		/// The window that's being destroyed
		/// </summary>
		Ref<Window> Window;
	};

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

	private:
		/// <summary>
		/// The current WindowingModule instance
		/// </summary>
		static WeakRef<WindowingModule> s_Instance;

	public:
		/// <summary>
		/// Gets the backend for this WindowingModule
		/// </summary>
		/// <returns>The backend for this WindowingModule</returns>
		Ref<WindowingBackend> GetBackend() const { return m_WindowingBackend; }

		/// <summary>
		/// Creates a window
		/// </summary>
		/// <param name="createParams">The parameters for the window</param>
		/// <returns>The created window</returns>
		WeakRef<Window> CreateWindow(const WindowCreateParams& createParams);

		/// <summary>
		/// Destroys a window
		/// </summary>
		/// <param name="window">The window to destroy</param>
		virtual void DestroyWindow(Ref<Window> window);

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
		/// Gets a list of all monitors attached to the system
		/// </summary>
		/// <returns>A list of monitors attached to the system</returns>
		List<Ref<Monitor>> GetMonitors() const { return m_WindowingBackend->GetMonitors(); }

	private:
		/// <summary>
		/// Creates the windowing backend
		/// </summary>
		/// <param name="backendAPI">The backend API to create</param>
		void CreateBackend(WindowingAPI backendAPI);

	public:
		/// <summary>
		/// Invoked when a window gets created
		/// </summary>
		EventSource<WindowCreatedEvent> OnWindowCreated;

		/// <summary>
		/// Invoked when a window is about to be destroyed
		/// </summary>
		EventSource<WindowDestroyedEvent> OnWindowDestroyed;

	private:
		/// <summary>
		/// The backend this WindowingModule uses
		/// </summary>
		Ref<WindowingBackend> m_WindowingBackend;

		/// <summary>
		/// The main window of the application
		/// </summary>
		Ref<Window> m_MainWindow;

		/// <summary>
		/// All the windows currently in-use
		/// </summary>
		List<Ref<Window>> m_Windows;

		/// <summary>
		/// A list of windows whose destruction is imminent
		/// </summary>
		List<Ref<Window>> m_DeferredWindowsToDestroy;
	};
}