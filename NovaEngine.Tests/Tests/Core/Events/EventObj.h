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

	static void StaticCallback(Nova::Event& e)
	{
		StaticEventsReceived++;
	}

	int EventsReceived = 0;
	static int StaticEventsReceived;

private:
	bool m_StopPropagation = false;
};

int EventObj::StaticEventsReceived = 0;