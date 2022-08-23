#include "Exception.h"

namespace Nova
{
	Exception GetException()
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