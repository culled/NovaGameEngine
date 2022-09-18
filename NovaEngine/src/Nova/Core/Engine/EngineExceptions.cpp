#include "EngineExceptions.h"

namespace Nova
{
	EngineInitException::EngineInitException(const string& what) :
		Exception(what)
	{}
}