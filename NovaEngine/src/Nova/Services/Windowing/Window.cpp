#include "Window.h"

#include "WindowingService.h"
#include "Nova/Core/Engine/Engine.h"

namespace Nova::Windowing
{
	WindowCreateParams::WindowCreateParams(const Vector2i& initialSize, const string& title) :
		InitialSize(initialSize), Title(title), VSync(true), InitialPosition(Vector2i::MAX)
	{}

	WindowClosingEvent::WindowClosingEvent() :
		Cancel(false)
	{}

	void Window::Close(bool forceClosed)
	{
		// Tell listeners we're closing
		WindowClosingEvent e;
		OnClosing.Emit(e);

		// Close the window if we need to
		if (!e.Cancel || forceClosed)
		{
			OnClosed.EmitAnonymous();
			CloseInternal();
		}
	}

	void Window::CloseInternal()
	{
		// Queue this window to be destroyed on the next tick
		if (auto windowService = Engine::Get()->GetService<WindowingService>())
		{
			windowService->QueueWindowRelease(GetSelfRef<Window>());
		}
	}
}