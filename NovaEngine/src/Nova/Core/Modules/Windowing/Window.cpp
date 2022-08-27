#include "Window.h"

namespace Nova::Windowing
{
	WindowClosingEvent::WindowClosingEvent() :
		ShouldClose(true)
	{}

	WindowResizedEvent::WindowResizedEvent(Vector2i newSize) :
		NewSize(newSize)
	{}

	WindowCreateParams::WindowCreateParams(const Vector2i& size, const string& title) :
		InitialSize(size), Title(title), VSync(true), InitialPosition(Vector2i::ZERO),
		ShareGfxContext(nullptr)
	{}
}