#pragma once

#include "Nova/Core/Engine.h"

namespace Nova
{
	/// <summary>
	/// Base struct for all events
	/// </summary>
	NovaStruct Event
	{
		/// <summary>
		/// Constructor for the event
		/// </summary>
		Event();
		virtual ~Event() = default;

		// If set to false, this event will stop propagating to listeners
		bool Propagate = true;
	};
}