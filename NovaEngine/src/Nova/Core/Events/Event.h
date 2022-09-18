#pragma once

#include "Nova/Core/EngineAPI.h"

namespace Nova
{
	/// <summary>
	/// Base struct for all events
	/// </summary>
	struct NovaAPI Event
	{
		/// <summary>
		/// Constructor for the event
		/// </summary>
		Event();
		virtual ~Event() = default;

		/// <summary>
		/// If set to false, this event will stop propagating to subsequent listeners
		/// </summary>
		bool Propagate = true;
	};
}