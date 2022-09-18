#pragma once

#include "Nova/Core/EngineAPI.h"
#include "SwapChain.h"

namespace Nova::Rendering
{
	/// <summary>
	/// A list of rendering APIs
	/// </summary>
	enum class RenderingAPI
	{
		Vulkan
	};

	class NovaAPI RenderingBackend
	{
	public:
		/// <summary>
		/// Gets the RenderingAPI that this backend uses
		/// </summary>
		/// <returns></returns>
		virtual RenderingAPI GetAPI() const = 0;

		/// <summary>
		/// Creates a swapchain
		/// </summary>
		/// <returns>A created swapchain</returns>
		virtual Ref<SwapChain> CreateSwapchain() = 0;
	};
}