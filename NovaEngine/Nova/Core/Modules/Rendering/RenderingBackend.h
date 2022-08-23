#pragma once

#include "Nova/Core/Engine.h"
#include "GraphicsContext.h"
#include "Nova/Core/Modules/Windowing/Window.h"

namespace Nova::Rendering
{
	/// <summary>
	/// A list of rendering APIs
	/// </summary>
	enum class RenderingBackendAPI
	{
		OpenGL
	};

	/// <summary>
	/// Base class for a rendering backend
	/// </summary>
	class NovaAPI RenderingBackend : public RefCounted
	{
	public:
		RenderingBackend() = default;
		virtual ~RenderingBackend() = default;
		
	public:
		/// <summary>
		/// Gets the rendering API this backend uses
		/// </summary>
		/// <returns>This backend's rendering API</returns>
		virtual RenderingBackendAPI GetAPI() const = 0;

		/// <summary>
		/// Creates a graphics context
		/// </summary>
		/// <returns>A new graphics context</returns>
		virtual Ref<GraphicsContext> CreateGraphicsContext() = 0;
	};
}