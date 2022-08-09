#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Events/TickListener.h"

namespace Nova
{
	/// <summary>
	/// Base class for all application modules
	/// </summary>
	NovaClass AppModule : public TickListener
	{
	public:
		/// <summary>
		/// Constructor for this app module
		/// </summary>
		/// <param name="additionalTickOrderOffset">An additional offset to add to the default offset for the module</param>
		AppModule(int additionalTickOrderOffset = 0);
		virtual ~AppModule() = default;

	protected:
		/// <summary>
		/// Gets the default tick order for the module
		/// </summary>
		/// <returns>The default tick order for this module</returns>
		virtual int GetDefaultTickOrder() const = 0;

		/// <summary>
		/// Gets the tick order for this module, accounting for the additional offset supplied by the constructor
		/// </summary>
		/// <returns>The final tick order for this module</returns>
		virtual int GetTickOrder() const override { return GetDefaultTickOrder() + m_TickOrderOffset; }

	private:
		int m_TickOrderOffset;
	};
}