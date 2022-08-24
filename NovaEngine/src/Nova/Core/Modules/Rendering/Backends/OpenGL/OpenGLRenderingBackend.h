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
		OpenGLRenderingBackend();
		~OpenGLRenderingBackend();

	// RenderingBackend ----------
	public:
		virtual RenderingBackendAPI GetAPI() const override { return RenderingBackendAPI::OpenGL; }
		virtual void InitGraphicsContext(Ref<GraphicsContext> context) override;
		virtual List<Ref<GraphicsContext>> GetActiveGraphicsContexts() override;
		virtual void LoadExtensionsForGraphicsContext(Ref<GraphicsContext> context) override;

	// RenderingBackend ----------

	private:
		/// <summary>
		/// Weak references to all created graphics contexts
		/// </summary>
		List<WeakRef<GraphicsContext>> m_GraphicsContexts;
	};
}