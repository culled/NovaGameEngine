#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

namespace Nova::Rendering
{
    /// <summary>
    /// Base class for a render layer.
    /// A RenderLayer defines what kinds of things get rendered to it. Subsequent layers render over earlier layers to create a final, composited image
    /// </summary>
    class NovaAPI RenderLayer : public RefCounted
    {
    public:
        RenderLayer() = default;
        virtual ~RenderLayer() = default;

    public:
        /// <summary>
        /// Called when this layer should start rendering a frame
        /// </summary>
        virtual void BeginFrame(Ref<GraphicsContext> currentContext, double deltaTime) = 0;

        /// <summary>
        /// Called when this layer should finish rendering a frame
        /// </summary>
        virtual void EndFrame(Ref<GraphicsContext> currentContext, double deltaTime) = 0;
    };
}

