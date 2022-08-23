#include "ConsoleLogSink.h"

#include <iostream>

namespace Nova
{
	ConsoleLogSink::ConsoleLogSink(LogLevel minimumLevel) :
		LogSink(minimumLevel)
	{}

	// LogSink ----------
	void ConsoleLogSink::WriteImpl(const string& message, LogLevel level)
	{
		std::cout << message << std::endl;
	}

	// LogSink ----------
}