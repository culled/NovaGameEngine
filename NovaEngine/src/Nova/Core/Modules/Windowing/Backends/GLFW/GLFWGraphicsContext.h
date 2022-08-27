#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

// Forward-declare so we don't include GLFW here
struct GLFWwindow;

namespace Nova::Windowing
{
	class NovaAPI GLFWGraphicsContext : public Rendering::GraphicsContext
	{
	public:
		GLFWGraphicsContext(GLFWwindow* internalContext, bool isShared);
		~GLFWGraphicsContext() = default;

		// RefCounted ----------
	protected:
		virtual void Init() override;

		// RefCounted ----------

		// GraphicsContext ----------
	public:
		virtual void MakeCurrent() override;
		virtual void ReleaseCurrent() override;

		virtual void* GetExtensionsFunction() const override;

		virtual void SwapBuffers() override;

		virtual Vector2i GetFramebufferSize() const override;

		virtual void* GetPlatformHandle() const override { return m_InternalContext; }

		virtual bool GetIsShared() const { return m_IsShared; }

		// GraphicsContext ----------

		/// <summary>
		/// Sets the window context that this context is linked to
		/// </summary>
		/// <param name="window">The window and context</param>
		void SetInternalContext(GLFWwindow* window) { m_InternalContext = window; }

		/// <summary>
		/// Gets the window context that this context is linked to
		/// </summary>
		/// <returns>The window context this GraphicsContext is linked to</returns>
		GLFWwindow* GetInternalContext() const { return m_InternalContext; }

	private:
		/// <summary>
		/// Ensures we have a valid internal context
		/// </summary>
		void EnsureInternalContext() const;

	private:
		/// <summary>
		/// The internal graphics context
		/// </summary>
		GLFWwindow* m_InternalContext;

		/// <summary>
		/// The shared state of this context
		/// </summary>
		bool m_IsShared = false;
	};
}