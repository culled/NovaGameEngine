#pragma once

#include "Nova/Core/EngineAPI.h"

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
	class NovaAPI Logger
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
		void AddSink(const Ref<LogSink>& sink);

		/// <summary>
		/// Writes to this log
		/// </summary>
		/// <param name="message">The message to write</param>
		/// <param name="level">The log level of the message</param>
		void Write(const string& message, LogLevel level = LogLevel::Info);

		/// <summary>
		/// Writes to this log
		/// </summary>
		/// <param name="message">The message to write</param>
		/// <param name="level">The log level of the message</param>
		template<typename ... Args>
		void WriteFormatted(LogLevel level, const string& formattedMessage, Args&&... values)
		{
			Write(FormatString(formattedMessage, std::forward<Args>(values)...), level);
		}

		/// <summary>
		/// Gets the name of this logger
		/// </summary>
		/// <returns>The name of this logger</returns>
		const string& GetName() const { return m_Name; }

	private:
		/// <summary>
		/// The name of this logger
		/// </summary>
		const string m_Name;

		/// <summary>
		/// The list of sinks this logger will write to
		/// </summary>
		List<Ref<LogSink>> m_Sinks;
	};
}