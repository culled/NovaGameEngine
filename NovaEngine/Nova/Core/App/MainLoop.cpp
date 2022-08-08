#include "MainLoop.h"

#include "App.h"
#include <algorithm>

namespace Nova
{
	void MainLoop::Start()
	{
		m_IsRunning = true;

		while (m_IsRunning)
		{
			TickListeners();
		}
	}

	void MainLoop::Stop()
	{
		m_IsRunning = false;
	}

	void MainLoop::AddTickListener(Ref<TickListener> listener)
	{
		WeakRef<TickListener> weakRef = MakeWeakRef<TickListener>(listener);
		m_TickListeners.push_back(weakRef);
		m_ListenersModifiedSinceLastTick = true;
	}

	void MainLoop::RemoveTickListener(Ref<TickListener> listener)
	{
		auto it = std::find_if(m_TickListeners.begin(), m_TickListeners.end(), [listener](const WeakRef<TickListener>& other) {
			auto otherPtr = other.lock();
			if (otherPtr)
			{
				return listener.get() == otherPtr.get();
			}
			else
			{
				return false;
			}
		});

		if (it != m_TickListeners.end())
		{
			m_TickListeners.erase(it);
		}
	}

	void MainLoop::SortTickListeners()
	{
		std::sort(m_TickListeners.begin(), m_TickListeners.end(), &MainLoop::CompareTickListeners);
		m_ListenersModifiedSinceLastTick = false;
	}

	bool MainLoop::CompareTickListeners(const WeakRef<TickListener>& lhs, const WeakRef<TickListener>& rhs)
	{
		auto lhsPtr = lhs.lock();
		auto rhsPtr = rhs.lock();

		if (lhsPtr && rhsPtr)
		{
			return lhsPtr->GetTickOrder() < rhsPtr->GetTickOrder();
		}
		else
		{
			if (lhsPtr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	void MainLoop::TickListeners()
	{
		// HACK: Quit if no listeners to handle, as we'd have an infinite loop with nothing to handle it
		if (m_TickListeners.size() == 0)
		{
			Stop();
			return;
		}

		// Only sort listeners if there's more than one and the list has changed since we last sorted
		if (m_TickListeners.size() > 1 && m_ListenersModifiedSinceLastTick)
		{
			// Sort all listeners so they're ticked in the correct order
			SortTickListeners();
		}

		// Copy listeners in-case the list changes mid-tick
		List<WeakRef<TickListener>> listeners(m_TickListeners);

		// Calculate the delta time between ticks
		TimeSpan currentTime = TimeSpan::Now();
		double deltaTime = (currentTime - m_LastTickTime).GetTotalSeconds();
		m_LastTickTime = currentTime;

		// Tick every listener
		for (const auto& listener : listeners)
		{
			auto listenerPtr = listener.lock();

			// TODO: remove dead listener pointers
			if (listenerPtr)
			{
				listenerPtr->Tick(deltaTime);
			}
		}
	}
}