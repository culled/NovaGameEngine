#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Events/TickListener.h"

namespace Nova
{
	NovaClass AppModule : public TickListener
	{
	public:
		AppModule(int tickOrderOffset = 0);
		virtual ~AppModule() = default;

	protected:
		virtual int GetDefaultTickOrder() const = 0;
		virtual int GetTickOrder() const override { return GetDefaultTickOrder() + m_TickOrderOffset; }

	private:
		int m_TickOrderOffset;
	};
}