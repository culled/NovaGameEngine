#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Services/EngineService.h"
#include "RenderingBackend.h"

namespace Nova::Rendering
{
	/// <summary>
	/// A service that provides rendering capabilities
	/// </summary>
	class NovaAPI RenderingService : public EngineService
	{
	public:
		RenderingService(RenderingAPI backendAPI);
		~RenderingService();

	public:
		void Tick(double deltaTime);
	};
}