#include "Exception.h"

namespace Nova
{
	Exception::Exception(const string& message) : 
		std::exception(message.c_str())
	{}

	Exception Exception::GetException()
	{
		try
		{
			throw;
		}
		catch (const Exception& ex)
		{
			return ex;
		}

		return Exception("Unknown exception");
	}
}