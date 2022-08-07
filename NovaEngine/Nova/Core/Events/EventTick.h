#pragma once

#include "Nova/Core/Engine.h"
#include "Event.h"

namespace Nova
{
	NovaStruct EventTick : public Event
	{
	public:
		double DeltaTime;
	};
}