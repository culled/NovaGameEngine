#pragma once

#include "Nova/Core/Engine.h"

namespace Nova::ImGui
{
	/// <summary>
	/// Base class of an ImGui rendering backend
	/// </summary>
	class NovaAPI ImGuiRenderer
	{
	public:
		ImGuiRenderer() = default;
		virtual ~ImGuiRenderer() = default;

	public:
		/// <summary>
		/// Begins a new frame
		/// </summary>
		virtual void BeginFrame() = 0;

		/// <summary>
		/// Ends the drawing of the frame
		/// </summary>
		virtual void EndFrame() = 0;
	};
}