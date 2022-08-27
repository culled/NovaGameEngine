#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

namespace Nova::Rendering
{
    /// <summary>
    /// A list of default render layer IDs
    /// </summary>
    enum class DefaultRenderLayerIDs {
        ImGui = 100
    };

    /// <summary>
    /// Base class for a render layer.
    /// A RenderLayer defines what kinds of things get rendered to it. Subsequent layers render over earlier layers to create a final, composited image
    /// </summary>
    class NovaAPI RenderLayer : public RefCounted
    {
    public:
        virtual ~RenderLayer() = default;

    public:
        /// <summary>
        /// Called when this layer should start rendering a frame
        /// </summary>
        /// <param name="deltaTime">The time between this frame and the last (in seconds)</param>
        virtual void BeginFrame(double deltaTime) = 0;

        /// <summary>
        /// Called when this layer has ended rendering a frame
        /// </summary>
        virtual void EndFrame() = 0;

        /// <summary>
        /// Called when this layer should render for a given GraphicsContext
        /// </summary>
        /// <param name="context">The context to render</param>
        virtual void RenderContext(Ref<GraphicsContext> context) = 0;

        /// <summary>
        /// Gets the unique ID of this render layer
        /// </summary>
        /// <returns>The ID of this render layer</returns>
        virtual int GetID() const = 0;
    };
}

