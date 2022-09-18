#include "TickListener.h"

namespace Nova
{
	void TickListener::NotifyTick(double deltaTime)
	{
		// Early out if we're not listening for ticks
		if (!m_ListenForTicks)
		{
			// Reset the time so that we respond properly if ticks re-enable later
			m_TimeSinceLastTick = 0.0;
			return;
		}

		m_TimeSinceLastTick += deltaTime;

		// Tick if enough time has passed
		if (m_TimeSinceLastTick >= m_TickPeriod)
		{
			m_TickFunc(m_TimeSinceLastTick);
			m_TimeSinceLastTick = 0.0;
		}
	}
}