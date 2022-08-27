#include "Monitor.h"

namespace Nova::Windowing
{
	MonitorVideoMode::MonitorVideoMode(Vector2i size, int bitDepthRed, int bitDepthGreen, int bitDepthBlue, int refreshRate) :
		Size(size), BitDepthRed(bitDepthRed), BitDepthGreen(bitDepthGreen), BitDepthBlue(bitDepthBlue), RefreshRate(refreshRate)
	{}
}