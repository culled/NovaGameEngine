#include "ConsoleLogSink.h"

#include <iostream>

namespace Nova
{
	ConsoleLogSink::ConsoleLogSink()
	{}

	void ConsoleLogSink::Write(const string& message, LogLevel level)
	{
		std::cout << message << std::endl;
	}
}