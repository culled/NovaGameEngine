#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Events/Event.h"

namespace Nova
{
	NovaClass Object
	{
	public:
		Object();
		virtual ~Object() = default;

		void SendEvent(Ref<Event> e);

	protected:
		virtual void Tick();

	private:
		bool m_ListenForTick = true;
	};
}