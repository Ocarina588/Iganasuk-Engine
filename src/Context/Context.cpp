#include "Context.hpp"

namespace ig = iganasuk;

ig::Context::Context(int w, int h, std::string const& title_name) : __window(w, h, title_name)
{
	ig::Context::singleton(true, this);
	uint32_t count;
	char const** tmp = glfwGetRequiredInstanceExtensions(&count);
	for (uint32_t i = 0; i < count; i++)
		__instance.add_extension(tmp[i]);

	__instance.add_extension("VK_EXT_debug_utils");
	__instance.add_layer("VK_LAYER_KHRONOS_validation");
	__instance.add_layer("VK_LAYER_LUNARG_monitor");
	__instance.init();
	
	__surface.init(__instance, __window);
	
	__device.add_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	//__device.add_extension(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
	//__device.add_extension(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
	//__device.add_extension(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
	//__device.add_extension(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
	//__device.add_extension(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
	//__device.add_extension(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
	//__device.add_extension(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);
	__device.init(__instance, __surface);

	__swapchain.init(__device.info, __window, __surface);

	__render_pass.init();

	__swapchain.init_framebuffers(__render_pass);
}

ig::Context::~Context(void)
{

}

ig::Context &ig::Context::singleton(bool store, ig::Context* t)
{
	static std::unique_ptr<ig::Context> data = nullptr;

	if (store)
		data = std::unique_ptr<ig::Context>(t);
	if (!data)
		throw std::runtime_error("need to initialize Context");

	return *data;
}

void ig::Context::init(int w, int h, std::string const& title_name)
{
	new ig::Context(w, h, title_name);
}