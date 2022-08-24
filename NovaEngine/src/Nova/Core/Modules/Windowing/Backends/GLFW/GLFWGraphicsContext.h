#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Nova::Windowing
{
	class NovaAPI GLFWGraphicsContext : public Rendering::GraphicsContext
	{
	public:
		GLFWGraphicsContext(uint32_t width, uint32_t height);
		~GLFWGraphicsContext() = default;

		// GraphicsContext ----------
	public:
		virtual void MakeCurrent() override;
		virtual void* GetExtensionsFunction() const override;
		virtual void SwapBuffers() override;

		virtual void SetWidth(uint32_t width) override { m_Width = width; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual void SetHeight(uint32_t height) override { m_Height = height; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetNodeTreeContext(Ref<NodeTree> nodeTree) override { m_NodeTreeContext = nodeTree; }
		virtual Ref<NodeTree> GetNodeTreeContext() const override { return m_NodeTreeContext; }

		// GraphicsContext ----------

		/// <summary>
		/// Sets the window context that this context is linked to
		/// </summary>
		/// <param name="window">The window and context</param>
		void SetInternalContext(GLFWwindow* window) { m_InternalContext = window; }

	private:
		/// <summary>
		/// The width of this graphics context
		/// </summary>
		uint32_t m_Width;

		/// <summary>
		/// The height of this graphics context
		/// </summary>
		uint32_t m_Height;

		/// <summary>
		/// The node tree this context is associated with
		/// </summary>
		Ref<NodeTree> m_NodeTreeContext;

		/// <summary>
		/// The internal graphics context
		/// </summary>
		GLFWwindow* m_InternalContext;
	};
}