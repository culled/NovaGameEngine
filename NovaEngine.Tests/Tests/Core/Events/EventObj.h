#pragma once

#include <Nova/Core/Events/Event.h>

class EventObj
{
public:
	EventObj(bool stopPropagation = false) : m_StopPropagation(stopPropagation)
	{}

	void Callback(Nova::Ref<Nova::Event> e)
	{
		EventsReceived++;

		if (m_StopPropagation)
			e->Propagate = false;
	}

	int EventsReceived = 0;

private:
	bool m_StopPropagation = false;
};