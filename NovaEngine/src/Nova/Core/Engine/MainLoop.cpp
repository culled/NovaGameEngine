#include "MainLoop.h"

#include <algorithm>
#include <thread>
#include <chrono>

namespace Nova
{
	MainLoop::MainLoop(int targetTickrate) :
		m_LastTickTime(DateTime::Now() - TimeSpan::FromSeconds(s_StartupTickDuration)),
		m_CurrentDeltaTime(s_StartupTickDuration),
		m_TargetTickrate(targetTickrate)
	{}

	MainLoop::~MainLoop()
	{
		// Let all tick listeners clean up
		m_TickListeners.clear();
	}


	bool MainLoop::CompareTickListeners(const Ref<TickListener>& lhs, const Ref<TickListener>& rhs)
	{
		// Whichever has the lower tick order should be first
		return lhs->GetTickOrder() < rhs->GetTickOrder();
	}

	// HACK: to prevent a delta time of 0 on the first tick, set the last tick time to be before the current tick time
	const double MainLoop::s_StartupTickDuration = 1.0 / 60.0;

	void MainLoop::AddTickListener(const Ref<TickListener>& listener)
	{
		m_TickListeners.push_back(listener);

		m_IsListenerSortDirty = true;
	}

	void MainLoop::RemoveTickListener(const Ref<TickListener>& listener)
	{
		auto it = std::find_if(m_TickListeners.begin(), m_TickListeners.end(), [listener](const Ref<TickListener>& other) {
			return listener.get() == other.get();
			});

		if (it != m_TickListeners.end())
		{
			m_TickListeners.erase(it);
		}
	}

	void MainLoop::Run()
	{
		m_IsRunning = true;

		while (m_IsRunning)
		{
			if (m_TargetTickrate > 0)
			{
				WaitForTargetTickrate();
			}

			PerformTick();
		}
	}

	void MainLoop::Stop()
	{
		m_IsRunning = false;
	}

	void MainLoop::WaitForTargetTickrate()
	{
		DateTime nextTickTime = m_LastTickTime + TimeSpan::FromSeconds(1.0 / (double)m_TargetTickrate);
		TimeSpan waitTime = nextTickTime - DateTime::Now();

		if (waitTime.GetMicroseconds() > 0)
		{
			// https://stackoverflow.com/questions/4184468/sleep-for-milliseconds
			std::this_thread::sleep_for(std::chrono::microseconds(waitTime.GetMicroseconds()));
		}
	}

	void MainLoop::SortTickListeners()
	{
		std::sort(m_TickListeners.begin(), m_TickListeners.end(), &MainLoop::CompareTickListeners);
		m_IsListenerSortDirty = false;
	}

	void MainLoop::PerformTick()
	{
		// HACK: Quit if no listeners to handle, as we'd have an infinite loop with nothing to handle it
		if (m_TickListeners.size() == 0)
		{
			Stop();
			return;
		}

		// Only sort listeners if there's more than one and the list has changed since we last sorted
		if (m_TickListeners.size() > 1 && m_IsListenerSortDirty)
		{
			// Sort all listeners so they're ticked in the correct order
			SortTickListeners();
		}

		// Copy listeners in-case the list changes mid-tick
		List<Ref<TickListener>> listeners(m_TickListeners);

		// Calculate the delta time between ticks
		DateTime currentTime = DateTime::Now();
		m_CurrentDeltaTime = (currentTime - m_LastTickTime).GetTotalSeconds();
		m_LastTickTime = currentTime;

		// Tick every listener
		for (const auto& listener : listeners)
		{
			listener->NotifyTick(m_CurrentDeltaTime);
		}
	}
}