#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/List.h"
#include "Nova/Core/Types/String.h"
#include "Nova/Core/Types/RefCounted.h"
#include "Nova/Core/Types/DateTime.h"
#include "Nova/Core/Logging/Logger.h"
#include "Nova/Core/App/App.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Events/EventSource.h"
#include "Nova/Core/Services/EngineService.h"
#include "Nova/Core/Services/EngineServiceExceptions.h"
#include "MainLoop.h"
#include "AppExitCode.h"

#include <typeinfo>

namespace Nova
{
	/// <summary>
	/// The main engine class. An engine is created when the program runs and then the engine takes care of running the client application
	/// </summary>
	class NovaAPI Engine
	{
	public:
		~Engine();

	private:
		/// <summary>
		/// Creates an engine and passes it a list of arguments
		/// </summary>
		/// <param name="args">The list of arguments</param>
		Engine(const List<string>& args);

	public:
		/// <summary>
		/// Creates and initializes an engine instance
		/// </summary>
		/// <returns></returns>
		static Engine* Init(const List<string>& args);

		/// <summary>
		/// Gets the engine singleton instance
		/// </summary>
		/// <returns>The engine singleton</returns>
		static Engine* Get() { return s_Instance; }

	private:
		/// <summary>
		/// The singleton instance of the engine
		/// </summary>
		static Engine* s_Instance;

	public:
		/// <summary>
		/// Starts running the app
		/// </summary>
		/// <returns>The exit code of the app</returns>
		AppExitCode Run();

		/// <summary>
		/// Immediately stops the application
		/// </summary>
		void Stop(AppExitCode exitCode = AppExitCode::SUCCESS);

		/// <summary>
		/// Adds a TickListener to the main loop
		/// </summary>
		/// <param name="listener">The listener to add</param>
		void AddTickListener(const Ref<TickListener>& listener) { m_MainLoop->AddTickListener(listener); }

		/// <summary>
		/// Removes a TickListener from the main loop
		/// </summary>
		/// <param name="listener">The listener to remove</param>
		void RemoveTickListener(const Ref<TickListener>& listener) { m_MainLoop->RemoveTickListener(listener); }

		/// <summary>
		/// Logs to the engine logger
		/// </summary>
		/// <param name="level">The log level of the message</param>
		/// <param name="formatMessage">The message to log</param>
		/// <param name="values">Values to be integrated into the formatted message</param>
		template <typename ... Args>
		void Log(LogLevel level, const string& formatMessage, Args&& ... values) const
		{
			m_Logger->WriteFormatted(level, formatMessage, std::forward<Args>(values)...);
		}

		/// <summary>
		/// Creates an EngineService
		/// </summary>
		/// <param name="executionOffset">A user-defined offset to apply on top of the service's default offset</param>
		/// <param name="...args">Parameters to pass to the constructor of the service class</param>
		/// <returns>The created service, or nullptr if the service was unable to be created</returns>
		template<class ServiceClass, typename ... Args>
		ServiceClass* CreateService(Args&& ...args)
		{
			// Only accept classes that derive from EngineService
			static_assert(std::is_base_of<EngineService, ServiceClass>::value, "The class must inherit from EngineService");

			ServiceClass* service = GetService<ServiceClass>();

			if (service)
			{
				Log(LogLevel::Warning, "A service of type \"{0}\" already exists!", typeid(ServiceClass).name());
				return service;
			}

			// Create the service and add it to our list of services
			service = new ServiceClass(std::forward<Args>(args)...);
			m_Services.push_back(ManagedPtr<ServiceClass>(service));

			Log(LogLevel::Verbose, "Created service of type \"{0}\"", typeid(ServiceClass).name());

			return service;
		}

		/// <summary>
		/// Finds an active service of the given type
		/// </summary>
		/// <returns>A pointer to the active service, or nullptr if no service could be found</returns>
		template<typename ServiceClass>
		ServiceClass* GetService()
		{
			static_assert(std::is_base_of<EngineService, ServiceClass>::value, "The class must inherit from EngineService");

			// Find the service with the matching type
			auto it = std::find_if(m_Services.begin(), m_Services.end(), [](const ManagedPtr<EngineService>& service)
				{
					return typeid(*service.get()).hash_code() == typeid(ServiceClass).hash_code();
				});

			if (it != m_Services.end())
			{
				return static_cast<ServiceClass*>((*it).get());
			}

			return nullptr;
		}

		/// <summary>
		/// Gets the amount of time that the engine has been running
		/// </summary>
		/// <returns>The amount of time the engine has been running</returns>
		TimeSpan GetRunningTime() const;

		/// <summary>
		/// Sets the code that the app will return when it exits
		/// </summary>
		/// <param name="exitCode">The code that the app will exit with</param>
		void SetExitCode(AppExitCode exitCode) { m_ExitCode = exitCode; }

		/// <summary>
		/// Sets the target tickrate for the engine. If set to 0, then tick as fast as possible
		/// </summary>
		/// <param name="ticksPerSecond">The target number of ticks per second, or an unlimited speed if set to 0</param>
		void SetTargetTickrate(int ticksPerSecond) { m_MainLoop->SetTargetTickrate(ticksPerSecond); }

	private:
		/// <summary>
		/// Creates an engine singleton
		/// </summary>
		void CreateSingleton();

		/// <summary>
		/// Creates and initializes the app
		/// </summary>
		void CreateApp();

	public:
		/// <summary>
		/// Invoked when the application stops
		/// </summary>
		EventSource<Event> OnStop;

	private:
		/// <summary>
		/// A list of arguments that the engine started with
		/// </summary>
		const List<string> m_StartupArgs;

		/// <summary>
		/// Gets the time that the engine was started
		/// </summary>
		const DateTime m_StartTime;

		/// <summary>
		/// The code the application will exit with when it stops
		/// </summary>
		AppExitCode m_ExitCode;

		/// <summary>
		/// The app that is running
		/// </summary>
		Ref<App> m_App;

		/// <summary>
		/// The main loop that is running
		/// </summary>
		ManagedPtr<MainLoop> m_MainLoop;

		/// <summary>
		/// The engine's logger
		/// </summary>
		ManagedPtr<Logger> m_Logger;

		/// <summary>
		/// A list of running services for the engine
		/// </summary>
		List<ManagedPtr<EngineService>> m_Services;
	};
}