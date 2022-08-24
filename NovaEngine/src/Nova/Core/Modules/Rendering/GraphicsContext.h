#pragma once

#include "Nova/Core/Engine.h"

#include <functional>

namespace Nova::Rendering
{
	/// <summary>
	/// Base class for a graphics context
	/// </summary>
	class NovaAPI GraphicsContext : public RefCounted
	{
	public:
		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;

	public:
		/// <summary>
		/// Makes this graphics context current
		/// </summary>
		virtual void MakeCurrent() = 0;

		/// <summary>
		/// Loads extensions for this graphics context
		/// </summary>
		/// <param name="procFunc">The function that returns pointers to extension functions</param>
		virtual void LoadExtensions(void* procFunc) = 0;

		/// <summary>
		/// Swaps the front and back buffers of this context
		/// </summary>
		virtual void SwapBuffers() = 0;

		/// <summary>
		/// Sets the width of this graphics context
		/// </summary>
		/// <param name="width">The new width for the context</param>
		virtual void SetWidth(uint32_t width) = 0;

		/// <summary>
		/// Gets the width of this graphics context
		/// </summary>
		/// <returns>The width of this graphics context</returns>
		virtual uint32_t GetWidth() const = 0;

		/// <summary>
		/// Sets the height of this graphics context
		/// </summary>
		/// <param name="width">The new height for the context</param>
		virtual void SetHeight(uint32_t height) = 0;

		/// <summary>
		/// Gets the height of this graphics context
		/// </summary>
		/// <returns>The height of this graphics context</returns>
		virtual uint32_t GetHeight() const = 0;
	};
}