#pragma once

#include "Nova/Core/Engine.h"


namespace Nova
{
	NovaStruct Event
	{
	public:
		Event();
		virtual ~Event() = default;

	public:	
		bool Propagate = true;
	};
}