#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "utils.hpp"

namespace iganasuk {

	class Device {
	public:
		typedef struct info_s {
			std::vector<VkSurfaceFormatKHR> surface_formats;
			std::vector<VkPresentModeKHR> present_modes;
			VkSurfaceCapabilitiesKHR surface_capabilites{};

			VkPhysicalDeviceFeatures featues{};
			VkPhysicalDeviceProperties properties{};
			std::vector<VkExtensionProperties> extensions;

			std::vector<VkQueueFamilyProperties> queue_family_properties;
			uint32_t index_graphics = INVALID_UINT32;
			uint32_t index_present = INVALID_UINT32;
			uint32_t index_compute = INVALID_UINT32;
			uint32_t index_transfer = INVALID_UINT32;

			VkDevice ptr = nullptr;
			VkPhysicalDevice physical_ptr = nullptr;
		} info_t;
	public:
		Device(void);
		~Device(void);

		void init(VkInstance instance, VkSurfaceKHR surface);
		void add_extension(char const* extension);

		operator VkDevice (void) { return info.ptr; }
		operator VkPhysicalDevice (void) { return info.physical_ptr; }

		void					bind_memory				(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize offset = 0);
		void					bind_memory				(VkImage image, VkDeviceMemory memory, VkDeviceSize offset = 0);

		VkImage					create_image			(VkImageCreateInfo &i);
		VkImageView				create_image_view		(VkImageViewCreateInfo &i);
		VkFramebuffer			create_framebuffer		(VkFramebufferCreateInfo& i);
		VkSwapchainKHR			create_swapchain		(VkSwapchainCreateInfoKHR& i);
		VkRenderPass			create_renderpass		(VkRenderPassCreateInfo& i);

		void					destroy_image			(VkImage image)				{ vkDestroyImage(info.ptr, image, nullptr); };
		void					destroy_image_view		(VkImageView view)			{ vkDestroyImageView(info.ptr, view, nullptr); };
		void					destroy_framebuffer		(VkFramebuffer buffer)		{ vkDestroyFramebuffer(info.ptr, buffer, nullptr); }
		void					destroy_swapchain		(VkSwapchainKHR s)			{ vkDestroySwapchainKHR(info.ptr, s, nullptr); }
		void					destroy_render_pass		(VkRenderPass render_pass)	{ vkDestroyRenderPass(info.ptr, render_pass, nullptr); }

		uint32_t				find_memory_type		(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		VkDeviceMemory			allocate_memory			(VkMemoryAllocateInfo& i);

		void					free_memory				(VkDeviceMemory i) { vkFreeMemory(info.ptr, i, nullptr); }

		VkMemoryRequirements	get_memory_requirments	(VkBuffer buffer);
		VkMemoryRequirements	get_memory_requirments	(VkImage image);
		
		info_t info{};

		VkQueue queue_graphics = nullptr;
		VkQueue queue_present = nullptr;
		VkQueue queue_compute = nullptr;
		VkQueue queue_transfer = nullptr;
	private:

		std::vector<char const*> __extensions;

		info_t	get_physical_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
		bool	suitable(info_t const &inf);


		
	};
}
