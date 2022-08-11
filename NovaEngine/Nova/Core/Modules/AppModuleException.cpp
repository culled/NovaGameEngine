#include "AppModuleException.h"

namespace Nova
{
	AppModuleInitException::AppModuleInitException(const string& error) : 
		Exception(error.c_str())
	{}
}