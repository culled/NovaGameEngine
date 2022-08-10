#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Display/DisplayModule.h"

namespace Nova::Display
{
	NovaClass GLFWDisplayModule : public DisplayModule
	{
	private:
		static void InitGLFW();
		static void TerminateGLFW();

	public:
		GLFWDisplayModule(int tickOffset);
		~GLFWDisplayModule();

	protected:
		virtual void Tick(double deltaTime) override;
		virtual Ref<Window> CreateWindow(uint32_t width, uint32_t height, const string& title) override;

	private:
		static bool s_GLFWInitialized;

	};
}