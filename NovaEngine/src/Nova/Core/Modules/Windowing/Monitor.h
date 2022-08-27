#pragma once

#include "Nova/Core/Engine.h"

namespace Nova::Windowing
{
	/// <summary>
	/// A video mode for a monitor
	/// </summary>
	struct NovaAPI MonitorVideoMode
	{
		MonitorVideoMode(Vector2i size, int bitDepthRed, int bitDepthGreen, int bitDepthBlue, int refreshRate);

		Vector2i Size;
		int BitDepthRed;
		int BitDepthGreen;
		int BitDepthBlue;
		int RefreshRate;
	};

	/// <summary>
	/// Represents a monitor connected to a user's system
	/// </summary>
	class NovaAPI Monitor : public RefCounted
	{
	public:
		virtual ~Monitor() = default;

	public:
		/// <summary>
		/// Gets the name of this monitor
		/// </summary>
		/// <returns>The name of this monitor</returns>
		virtual string GetName() const = 0;
		
		/// <summary>
		/// Gets the position of this monitor's viewport on the virtual screen
		/// </summary>
		/// <returns>The position of this monitor's viewport on the virtual screen</returns>
		virtual Vector2i GetPosition() const = 0;

		/// <summary>
		/// Gets the physical size of this monitor
		/// </summary>
		/// <returns>The physical size of this monitor</returns>
		virtual Vector2i GetPhysicalSize() const = 0;

		/// <summary>
		/// Gets the sceen area of this monitor excluding the taskbar
		/// </summary>
		/// <returns>The work area of this monitor</returns>
		virtual Recti GetWorkArea() const = 0;

		/// <summary>
		/// Gets a list of video modes that this monitor supports
		/// </summary>
		/// <returns>A list of video modes that this monitor supports</returns>
		virtual List<MonitorVideoMode> GetSupportedVideoModes() const = 0;

		/// <summary>
		/// Gets the current video mode of this monitor
		/// </summary>
		/// <returns>The current video mode of this monitor</returns>
		virtual MonitorVideoMode GetCurrentVideoMode() const = 0;

		/// <summary>
		/// Gets if this monitor is the primary monitor for the system
		/// </summary>
		/// <returns>True if this monitor is the primary one in the system</returns>
		virtual bool IsPrimary() const = 0;

		/// <summary>
		/// Gets the dpi scale of this monitor
		/// </summary>
		/// <returns>The dpi scale of this monitor</returns>
		virtual Vector2 GetDpiScale() const = 0;
	};
}