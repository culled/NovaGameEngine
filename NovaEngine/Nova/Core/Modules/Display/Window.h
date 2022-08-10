#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Events/EventSource.h"

namespace Nova::Display
{
	NovaStruct WindowClosingEvent : public Event
	{
		bool ShouldClose = true;
	};

	NovaStruct WindowResizedEvent : public Event
	{
		WindowResizedEvent(uint32_t newWidth, uint32_t newHeight) :
			NewWidth(newWidth), NewHeight(newHeight) {}

		uint32_t NewWidth;
		uint32_t NewHeight;
	};

	NovaClass Window : public RefCounted
	{
	public:
		virtual ~Window() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const string& GetTitle() const = 0;

		virtual void SetVSyncEnabled(bool enabled) = 0;
		virtual bool GetVSyncEnabled() const = 0;

		virtual void Tick(double deltaTime) = 0;

	public:
		EventSource<WindowClosingEvent> OnClosing;
		EventSource<Event> OnClosed;
		EventSource<WindowResizedEvent> OnResized;
	};
}