#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Windowing/Window.h"

#include "Vendor/glfw/include/GLFW/glfw3.h"

namespace Nova::Windowing
{
	// Wrapper to add a destructor to the GLFWwindow type
	// https://stackoverflow.com/questions/35793672/use-unique-ptr-with-glfwwindow
	struct NovaAPI DestroyGLFWwindow {
		void operator()(GLFWwindow* ptr)
		{
			glfwDestroyWindow(ptr);
		}
	};

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
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const string& GetTitle() const override { return m_Title; }

		virtual void SetVSyncEnabled(bool enabled) override;
		virtual bool GetVSyncEnabled() const override { return m_VSync; }

		virtual void Close() override;

	// Window ----------

	private:
		/// <summary>
		/// Callback when the internal window attempts to close
		/// </summary>
		static void CloseCallback(GLFWwindow* window);

	private:
		/// <summary>
		/// Creates the internal GLFW window
		/// </summary>
		void CreateInternalWindow();

	private:
		/// The width of this window
		uint32_t m_Width;

		/// The height of this window
		uint32_t m_Height;

		/// The title of this window
		string m_Title;

		/// If vsync is enabled for this window
		bool m_VSync;

		/// Pointer the the GLFWwindow object
		std::unique_ptr<GLFWwindow, DestroyGLFWwindow> m_InternalWindow;
	};
}