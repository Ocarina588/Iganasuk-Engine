#include "Context/Instance.hpp"
#include "utils.hpp"

namespace ig = iganasuk;

ig::Instance::Instance(void)
{

}

ig::Instance::~Instance(void)
{
	if (__messenger)
		destroy_messenger();
	vkDestroyInstance(__ptr, nullptr);
}

void ig::Instance::init(void)
{
	VkInstanceCreateInfo create_info{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	auto debug_create_info = get_messenger_create_info();
	bool debug = std::find(__extensions.begin(), __extensions.end(), "VK_EXT_debug_utils") != __extensions.end();

	create_info.enabledExtensionCount = static_cast<uint32_t>(__extensions.size());
	create_info.ppEnabledExtensionNames = __extensions.data();

	if (debug) {
		create_info.ppEnabledLayerNames = __layers.data();
		create_info.enabledLayerCount = static_cast<uint32_t>(__layers.size());
		create_info.pNext = &debug_create_info;
	}
	if (vkCreateInstance(&create_info, nullptr, &__ptr) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance");
	
	if (debug)
		create_messenger();
}

void ig::Instance::add_extension(char const* extension)
{
	__extensions.push_back(extension);
}

void ig::Instance::add_layer(char const* layer)
{
	__layers.push_back(layer);
}

static VkBool32 VKAPI_PTR messenger_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	std::cout << TERMINAL_COLOR_CYAN << "Validation Layer: ";
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cout << TERMINAL_COLOR_YELLOW;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cout << TERMINAL_COLOR_RED;
		break;
	default:
		break;
	}
	std::cout << pCallbackData->pMessage << TERMINAL_COLOR_RESET << std::endl;
	return VK_SUCCESS;
}

VkDebugUtilsMessengerCreateInfoEXT ig::Instance::get_messenger_create_info(void) const
{
	VkDebugUtilsMessengerCreateInfoEXT create_info{ VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
	create_info.messageType = 0b1111;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.pfnUserCallback = messenger_callback;
	return create_info;
}

void ig::Instance::create_messenger(void)
{
	auto func = GET_INSTANCE_PROC(__ptr, PFN_vkCreateDebugUtilsMessengerEXT);
	auto debug = get_messenger_create_info();

	if (func(__ptr, &debug, nullptr, &__messenger) != VK_SUCCESS)
		throw std::runtime_error("failed to create debug messenger");
}

void ig::Instance::destroy_messenger(void)
{
	auto func = GET_INSTANCE_PROC(__ptr, PFN_vkDestroyDebugUtilsMessengerEXT);

	func(__ptr, __messenger, nullptr);
}