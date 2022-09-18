#include "EngineService.h"

#include "Nova/Core/Engine/Engine.h"

namespace Nova
{
	void EngineService::AddListenerToMainLoop(const Ref<TickListener>& listener) const
	{
		Engine::Get()->AddTickListener(listener);
	}

	void EngineService::RemoveListenerFromMainLoop(const Ref<TickListener>& listener) const
	{
		Engine::Get()->RemoveTickListener(listener);
	}
}
