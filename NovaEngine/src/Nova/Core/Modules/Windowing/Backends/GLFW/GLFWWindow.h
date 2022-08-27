#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Windowing/Window.h"
#include "GLFWGraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Nova::Windowing
{
	/// <summary>
	/// A GLFW implementation of a window
	/// </summary>
	class NovaAPI GLFWWindow : public Window
	{
	public:
		/// <summary>
		/// Creates a window with the given params
		/// </summary>
		/// <param name="createParams">The creation params for the window</param>
		GLFWWindow(const WindowCreateParams& createParams);

		virtual ~GLFWWindow();

	// Window ----------
	public:
		virtual void SetSize(const Vector2i& size) override;
		virtual Vector2i GetSize() const override;

		virtual void SetPosition(const Vector2i& position) override;
		virtual Vector2i GetPosition() const override;

		virtual void SetTitle(const string& title) override;
		virtual string GetTitle() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool GetVSync() const override { return m_VSync; }

		virtual void Show() override;
		virtual void Hide() override;
		virtual bool GetIsHidden() const override;

		virtual void Focus() override;
		virtual bool GetIsFocused() const override;

		virtual bool GetIsMinimized() const override;

		virtual void SetOpacity(double opacity) override;
		virtual double GetOpacity() const override;

		virtual Ref<Rendering::GraphicsContext> GetGraphicsContext() const override { return m_GraphicsContext; }

		virtual void* GetBackendWindowHandle() const override { return m_InternalWindowPtr; }
		virtual void* GetPlatformWindowHandle() const override;

		virtual void Close(bool forceClosed = false) override;

	protected:
		virtual void SetMainWindow(bool isMain) override { m_IsMainWindow = isMain; }

	// Window ----------

	private:
		/// <summary>
		/// Callback when the internal window attempts to close
		/// </summary>
		static void CloseCallback(GLFWwindow* window);

		/// <summary>
		/// Callback when the internal window is resized
		/// </summary>
		static void ResizeCallback(GLFWwindow* window, int newWidth, int newHeight);

	private:
		/// <summary>
		/// Creates the internal GLFW window
		/// </summary>
		void CreateInternalWindow(const WindowCreateParams& createParams);

	private:
		/// <summary>
		/// If vsync is enabled for this window
		/// </summary>
		bool m_VSync;

		/// <summary>
		/// If this window is the main one for the application
		/// </summary>
		bool m_IsMainWindow = false;

		/// <summary
		/// Pointer the the GLFWwindow object
		/// </summary>
		GLFWwindow* m_InternalWindowPtr;

		/// <summary>
		/// This window's graphics context
		/// </summary>
		Ref<Rendering::GraphicsContext> m_GraphicsContext;
	};
}