#include "OpenGLRenderingBackend.h"

#include "OpenGLGraphicsContext.h"

namespace Nova::Rendering
{
	Ref<GraphicsContext> OpenGLRenderingBackend::CreateGraphicsContext()
	{
		return MakeRef<OpenGLGraphicsContext>();
	}
}