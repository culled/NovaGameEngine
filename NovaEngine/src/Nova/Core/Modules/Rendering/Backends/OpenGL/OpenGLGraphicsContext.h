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
		OpenGLGraphicsContext(uint32_t width, uint32_t height);
		~OpenGLGraphicsContext() = default;

	// GraphicsContext ----------
	public:
		virtual void MakeCurrent() override;
		virtual void LoadExtensions(void* procFunc) override;
		virtual void SwapBuffers() override;

		virtual void SetWidth(uint32_t width) override { m_Width = width; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual void SetHeight(uint32_t height) override { m_Height = height; }
		virtual uint32_t GetHeight() const override { return m_Height; }
	// GraphicsContext ----------

	public:
		/// <summary>
		/// For windowing systems that own a graphics context, this allows them to set a callback so when this
		/// graphics context is made current, it will use this callback instead
		/// </summary>
		/// <param name="func">The callback to use when this context is requested to be made current</param>
		void SetMakeCurrentCallback(std::function<void()> func);

		/// <summary>
		/// For windowing systems that own a graphics context, this allows them to set a callback so when this
		/// graphics context's buffers are swapped, it will use this callback instead
		/// </summary>
		/// <param name="func">The callback to use when this context is requested to swap buffers</param>
		void SetSwapBuffersCallback(std::function<void()> func);

	private:
		/// Function callback that makes this graphics context current
		std::function<void()> m_MakeCurrentDelegate;

		/// <summary>
		/// Function callback that swaps this graphics context's buffers
		/// </summary>
		std::function<void()> m_SwapBuffersDelegate;

		/// <summary>
		/// The width of this graphics context
		/// </summary>
		uint32_t m_Width;

		/// <summary>
		/// The height of this graphics context
		/// </summary>
		uint32_t m_Height;
	};
}