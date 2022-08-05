#pragma once

#include "Nova/Core/Engine.h"

namespace Nova
{
	NovaClass AppModule
	{
	public:
		AppModule(int additionalOffset = 0);
		virtual ~AppModule();
	};
}