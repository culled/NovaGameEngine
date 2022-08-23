#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

namespace Nova::Rendering
{
	/// <summary>
	/// An OpenGL graphics context
	/// </summary>
	class NovaAPI OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		OpenGLGraphicsContext() = default;
		~OpenGLGraphicsContext() = default;

	// GraphicsContext ----------
	public:
		virtual void MakeCurrent() override;
		virtual void SetMakeCurrentCallback(MakeCurrentDelegate func) override;
		virtual void LoadExtensions(void* procFunc) override;

	// GraphicsContext ----------

	private:
		/// Function callback that makes this graphics context current
		MakeCurrentDelegate m_MakeCurrentDelegate;
	};
}