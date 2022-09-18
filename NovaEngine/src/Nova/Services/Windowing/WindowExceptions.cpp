#include "WindowExceptions.h"

namespace Nova
{
	WindowCreateException::WindowCreateException(const string& error) : Exception(error)
	{}
}