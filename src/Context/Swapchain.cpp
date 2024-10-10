#include <algorithm>
#include "Context.hpp"

namespace ig = iganasuk;

ig::Swapchain::Swapchain(void)
{

}

ig::Swapchain::~Swapchain(void)
{
	for (int i = 0; i < __views.size(); i++) {
		ig_device.destroy_image_view(__views[i]);
	}
	ig_device.destroy_swapchain(__ptr);
}

void ig::Swapchain::init(ig::Device::info_t &info, GLFWwindow* window, VkSurfaceKHR surface)
{
	VkSwapchainCreateInfoKHR create_info{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	device_info = info;
	extent = get_extent(window);

	create_info.surface = surface;
	create_info.minImageCount = info.surface_capabilites.minImageCount + 1;
	if (info.surface_capabilites.maxImageCount != 0 && create_info.minImageCount > info.surface_capabilites.maxImageCount)
		create_info.minImageCount = info.surface_capabilites.maxImageCount;
	create_info.imageFormat = format.format;
	create_info.imageColorSpace = format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (info.index_graphics != info.index_present)
		throw std::runtime_error("need to enable sharing mode concurrent");
	create_info.preTransform = info.surface_capabilites.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	create_info.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(info.ptr, &create_info, nullptr, &__ptr) != VK_SUCCESS)
		throw std::runtime_error("failed to create swapchain");

	create_resources();
}

VkExtent2D ig::Swapchain::get_extent(GLFWwindow* window)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	VkExtent2D e = { static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
	e.width = std::clamp(e.width, device_info.surface_capabilites.minImageExtent.width, device_info.surface_capabilites.maxImageExtent.width);
	e.height = std::clamp(e.height, device_info.surface_capabilites.minImageExtent.height, device_info.surface_capabilites.maxImageExtent.height);
	return e;
}

void ig::Swapchain::create_resources(void)
{
	uint32_t count = 0;
	vkGetSwapchainImagesKHR(device_info.ptr, __ptr, &count, nullptr);
	__images.resize(count);
	__views.resize(count);
	vkGetSwapchainImagesKHR(device_info.ptr, __ptr, &count, __images.data());

	for (uint32_t i = 0; i < count; i++)
		__views[i] = ig::Image::create_view(__images[i], format.format, VK_IMAGE_ASPECT_COLOR_BIT);
	
	__depth_image = ig::Image::create(extent, VK_FORMAT_D32_SFLOAT, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_IMAGE_ASPECT_DEPTH_BIT);
}

void ig::Swapchain::init_framebuffers(VkRenderPass render_pass)
{
	__framebuffers.resize(__images.size());

	for (int i = 0; i < __images.size(); i++) {
		VkImageView attachments[] = { __views[i], *__depth_image };
		VkFramebufferCreateInfo create_info{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
		create_info.renderPass = render_pass;
		create_info.attachmentCount = 2;
		create_info.pAttachments = nullptr;
		create_info.width = extent.width;
		create_info.height = extent.height;
		create_info.layers = 1;
	}
}