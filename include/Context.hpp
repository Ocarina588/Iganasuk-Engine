#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Context/Instance.hpp"
#include "Context/Window.hpp"
#include "Context/Surface.hpp"
#include "Context/Device.hpp"
#include "Context/Swapchain.hpp"
#include "Context/RenderPass.hpp"
#include "Shader.hpp"
#include "Synchronisation.hpp"
#include "Command.hpp"
#include "Descriptor.hpp"
#include "Buffer.hpp"
#include "Pipeline.hpp"
#include "Model.hpp"
#include "Event.hpp"
#include "Camera.hpp"

namespace iganasuk {

	class Context {

	public:
		static void init(int w, int h, std::string const& title_name);
		Context(int w, int h, std::string const& title_name);
		~Context(void);

		static Window&		window(void)		{ return singleton().__window; }
		static Instance&	instance(void)		{ return singleton().__instance; }
		static Surface&		surface(void)		{ return singleton().__surface; }
		static Device&		device(void)		{ return singleton().__device; }
		static Swapchain&	swapchain(void)		{ return singleton().__swapchain; }
		static RenderPass&	render_pass(void)	{ return singleton().__render_pass; }
		static void			update_dt(void)		{
			static auto startTime = std::chrono::high_resolution_clock::now();

			auto currentTime = std::chrono::high_resolution_clock::now();
			singleton().dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		}
		static float		get_dt(void) { return singleton().dt; }

		float dt = 0.f;

	private:
		static Context& singleton(bool store = false, Context *t = nullptr);

		Window		__window;
		Instance	__instance;
		Surface		__surface;
		Device		__device;
		Swapchain	__swapchain;
		RenderPass	__render_pass;
	};

	#define ig_window		iganasuk::Context::window()
	#define ig_instance		iganasuk::Context::instance()
	#define ig_surface		iganasuk::Context::surface()
	#define ig_device		iganasuk::Context::device()
	#define ig_swapchain	iganasuk::Context::swapchain()
	#define ig_render_pass	iganasuk::Context::render_pass()

}