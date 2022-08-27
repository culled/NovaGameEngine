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
		/// Initializes a graphics context to use with this RenderingBackend
		/// </summary>
		/// <param name="context">The context to initialize</param>
		virtual void InitGraphicsContext(Ref<GraphicsContext> context) = 0;

		/// <summary>
		/// Gets a list of all the active graphics contexts for the application
		/// </summary>
		/// <returns>A list of all active graphics contexts</returns>
		virtual List<Ref<GraphicsContext>> GetGraphicsContexts() = 0;

		/// <summary>
		/// Loads graphics extensions for the given GraphicsContext
		/// </summary>
		/// <param name="context">The context to load extensions for</param>
		virtual void LoadExtensionsForGraphicsContext(Ref<GraphicsContext> context) = 0;
	};
}