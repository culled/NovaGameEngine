#include "WindowingBackendExceptions.h"

namespace Nova
{
	WindowingBackendInitException::WindowingBackendInitException(const string& error) : Exception(error)
	{}
}