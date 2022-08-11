#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Types/List.h"
#include "Nova/Core/Types/String.h"
#include "LogLevel.h"
#include "LogSink.h"

#include <memory>

namespace Nova
{
	/// <summary>
	/// Class that accepts LogSinks and writes log messages to them
	/// </summary>
	NovaClass Logger
	{
	public:
		/// <summary>
		/// Creates a logger with the given name
		/// </summary>
		/// <param name="name">The name for this logger</param>
		Logger(const string& name);

	public:
		/// <summary>
		/// Creates and adds a sink for logging
		/// </summary>
		/// <param name="...args">Arguments to pass to the sink class's constructor</param>
		template<class T, typename ... Args>
		void CreateSink(Args&& ...args)
		{
			// Only accept classes that derive from LogSink
			static_assert(std::is_base_of<LogSink, T>::value, "The class must inherit from LogSink");

			Ref<T> sink = MakeRef<T>(std::forward<Args>(args)...);
			AddSink(sink);
		}

		/// <summary>
		/// Adds a sink to this log
		/// </summary>
		/// <param name="sink">The sink to add</param>
		void AddSink(Ref<LogSink> sink);

		/// <summary>
		/// Writes to this log
		/// </summary>
		/// <param name="message">The message to write</param>
		/// <param name="level">The log level of the message</param>
		void Write(const string& message, LogLevel level = LogLevel::Info);

		/// <summary>
		/// Gets the name of this logger
		/// </summary>
		/// <returns>The name of this logger</returns>
		string GetName() const { return m_Name; }

	private:
		// The name of this logger
		const string m_Name;

		// The list of sinks this logger will write to
		List<Ref<LogSink>> m_Sinks;
	};
}