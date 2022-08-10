#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Display/Window.h"

#include "Vendor/glfw/include/GLFW/glfw3.h"

namespace Nova::Display
{
	NovaClass GLFWWindow : public Window
	{
	private:
		// Wrapper to add a destructor to the GLFWwindow type
		// https://stackoverflow.com/questions/35793672/use-unique-ptr-with-glfwwindow
		struct DestroyGLFWwindow {
			void operator()(GLFWwindow* ptr)
			{
				glfwDestroyWindow(ptr);
			}
		};

		static void CloseCallback(GLFWwindow* window);

	public:
		GLFWWindow(uint32_t width, uint32_t height, const string& title);
		virtual ~GLFWWindow();

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }
		virtual const string& GetTitle() const { return m_Title; }

		virtual void SetVSyncEnabled(bool enabled);
		virtual bool GetVSyncEnabled() const { return m_VSync; }

		virtual void Tick (double deltaTime) {}

	private:
		void CreateInternalWindow();

	private:
		uint32_t m_Width, m_Height;
		string m_Title;
		bool m_VSync;

		std::unique_ptr<GLFWwindow, DestroyGLFWwindow> m_InternalWindow;
	};
}