#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/RenderingBackend.h"

namespace Nova::Rendering
{
	/// <summary>
	/// A rendering backend using OpenGL
	/// </summary>
	class NovaAPI OpenGLRenderingBackend : public RenderingBackend
	{
	public:
		OpenGLRenderingBackend() = default;
		~OpenGLRenderingBackend() = default;

	// RenderingBackend ----------
	public:
		virtual RenderingBackendAPI GetAPI() const override { return RenderingBackendAPI::OpenGL; }
		virtual Ref<GraphicsContext> CreateGraphicsContext() override;

	// RenderingBackend ----------
	};
}