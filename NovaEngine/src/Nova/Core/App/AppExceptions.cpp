#include "AppExceptions.h"

namespace Nova
{
	AppInitException::AppInitException(const string& what) : 
		Exception(what)
	{}
}