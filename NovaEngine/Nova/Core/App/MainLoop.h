#pragma once

#include "Nova/Core/Engine.h"

namespace Nova
{
	NovaClass MainLoop
	{
	public:
		virtual void Run();

		virtual void Stop();

	protected:
		bool m_IsRunning = false;
	};
}