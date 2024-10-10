#pragma once

#include <iostream>
#include "Context/Device.hpp"
#include "Image.hpp"
#include "Synchronisation.hpp"

namespace iganasuk {

	class Swapchain {
	public:
		friend class RenderPass;
		
		Swapchain(void);
		~Swapchain(void);

		operator VkSwapchainKHR (void) { return __ptr; }
		operator VkSwapchainKHR* (void) { return &__ptr; }
		void init(Device::info_t &info, GLFWwindow* window, VkSurfaceKHR surface);
		VkExtent2D get_extent(GLFWwindow* window);
		void create_resources(void);
		void init_framebuffers(VkRenderPass render_pass);
		void acquire_next_image(Semaphore& s);
		VkFramebuffer get_actual_framebuffer(void) { return __framebuffers[image_index]; }

		uint32_t image_index = 0;
		VkExtent2D extent{};
		VkSurfaceFormatKHR format = { VK_FORMAT_B8G8R8A8_SRGB, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
	private:
		VkSwapchainKHR __ptr = nullptr;

		std::vector <VkImage> __images;
		std::vector <VkImageView> __views;
		std::vector <VkFramebuffer> __framebuffers;

		Image::handle __depth_image;

		Device::info_t device_info;

	};
}