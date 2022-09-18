#include "EngineServiceExceptions.h"

namespace Nova
{
	EngineServiceInitException::EngineServiceInitException(const string& error) :
		Exception(error)
	{}
}