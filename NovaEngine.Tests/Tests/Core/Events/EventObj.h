#pragma once

#include <Nova/Core/Events/Event.h>

class EventObj
{
public:
	EventObj(bool stopPropagation = false) : m_StopPropagation(stopPropagation)
	{}

	void Callback(Nova::Event& e)
	{
		EventsReceived++;

		if (m_StopPropagation)
			e.Propagate = false;
	}

	void AnotherCallback(Nova::Event& e)
	{
		EventsReceived += 2;
	}

	static void StaticCallback(Nova::Event& e)
	{
		StaticEventsReceived++;
	}

	static void AnotherStaticCallback(Nova::Event& e)
	{
		StaticEventsReceived += 2;
	}

	int EventsReceived = 0;
	static int StaticEventsReceived;

private:
	bool m_StopPropagation = false;
};

int EventObj::StaticEventsReceived = 0;