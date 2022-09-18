#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/RefCounted.h"

#include "LogLevel.h"

namespace Nova
{
	/// <summary>
	/// Base class for all sinks of log messages
	/// </summary>
	class NovaAPI LogSink : public RefCounted
	{
	public:
		/// <summary>
		/// Constructor for this LogSink
		/// </summary>
		/// <param name="minimumLevel">The minimum level that this sink should write messages at. Defaults to LogLevel::Verbose</param>
		LogSink(LogLevel minimumLevel);

		virtual ~LogSink() = default;

	public:
		/// <summary>
		/// Writes to this sink
		/// </summary>
		/// <param name="message">The message to write</param>
		/// <param name="level">The log level of the message</param>
		virtual void Write(const string& message, LogLevel level);

	protected:
		/// <summary>
		/// Gets if the given message level should be written for this sink
		/// </summary>
		/// <param name="level">The level of the message</param>
		/// <returns>True if this sink should write the message at the given level</returns>
		bool ShouldWrite(LogLevel level) const;

		/// <summary>
		/// Implementation for writing to this sink. Only called when the message passes the minimum log level to write
		/// </summary>
		/// <param name="message">The message to write</param>
		/// <param name="level">The level of the message</param>
		virtual void WriteImpl(const string& message, LogLevel level) = 0;

	private:
		/// <summary>
		/// The minimum level of messages that this sink will write
		/// </summary>
		LogLevel m_MinimumLevel = LogLevel::Verbose;
	};
}