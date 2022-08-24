#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/AppModule.h"
#include "Layers/RenderLayerStack.h"
#include "RenderingBackend.h"

namespace Nova::Rendering
{
	/// <summary>
	/// AppModule that renders images to an output
	/// </summary>
	class NovaAPI RenderModule : public AppModule
	{
	public:
		RenderModule(int tickOrderOffset, RenderingBackendAPI backendAPI);
		~RenderModule();

	// RefCounted ----------
	protected:
		virtual void Init() override;

	// RefCounted ----------
		
	// TickListener ----------
	public:
		virtual void Tick(double deltaTime) override;

	// TickListener ----------
	
	// AppModule ----------
	protected:
		virtual int GetDefaultTickOrder() const override
		{
			return static_cast<int>(AppModule::BuiltInModuleOffset::RenderingModule);
		}

	// AppModule ----------

	public:
		/// <summary>
		/// Gets the active RenderModule singleton
		/// </summary>
		/// <returns>A reference to the active RenderModule singleton</returns>
		static Ref<RenderModule> Get() { return s_Instance.lock(); }

	private:
		/// The singleton instance of the RenderModule
		static WeakRef<RenderModule> s_Instance;

	public:
		/// <summary>
		/// Gets the currently active rendering backend
		/// </summary>
		/// <returns>The current rendering backend</returns>
		Ref<RenderingBackend> GetBackend() { return m_RenderingBackend; }

		void AppendRenderLayer(Ref<RenderLayer> layer);
		void RemoveRenderLayer(Ref<RenderLayer> layer);

	private:
		/// <summary>
		/// Creates the rendering backend
		/// </summary>
		/// <param name="backendAPI">The backend API to create</param>
		void CreateBackend(RenderingBackendAPI backendAPI);

	private:
		/// The rendering backend this RenderModule is using
		Ref<RenderingBackend> m_RenderingBackend;

		/// <summary>
		/// The stack of RenderLayers
		/// </summary>
		RenderLayerStack m_RenderLayerStack;
	};
}