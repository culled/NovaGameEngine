// Windows entrypoint

#pragma once

#include "Nova/Core/Types/String.h"
#include "Nova/Core/Types/List.h"

namespace Nova
{
	extern int Entry(const Nova::List<Nova::string>& args);
}

int main(int argc, char** argv)
{
	Nova::List<Nova::string> args;

	// Convert the args into a list
	for (int i = 0; i < argc; i++)
		args.push_back(Nova::string(argv[i]));

	// Call the main entrypoint of the engine
	return Nova::Entry(args);
}