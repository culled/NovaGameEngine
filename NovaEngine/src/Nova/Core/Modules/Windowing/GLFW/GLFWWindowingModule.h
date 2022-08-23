#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Windowing/WindowingModule.h"

namespace Nova::Windowing
{
	/// <summary>
	/// A windowing module that uses GLFW as its backend
	/// </summary>
	class NovaAPI GLFWWindowingModule : public WindowingModule
	{
	public:
		GLFWWindowingModule(int tickOffset);
		~GLFWWindowingModule();

	// TickListener ----------
	public:
		virtual void Tick(double deltaTime) override;

	// TickListener ----------

	// WindowingModule ----------
	protected:
		virtual Ref<Window> CreateWindow(const WindowCreateParams& createParams) override;

	// WindowingModule ----------

	private:
		/// <summary>
		/// Initializes GLFW
		/// </summary>
		static void InitGLFW();

		/// <summary>
		/// Terminates GLFW
		/// </summary>
		static void TerminateGLFW();

	private:
		/// True if GLFW has been initialized and can be used
		static bool s_GLFWInitialized;
	};
}