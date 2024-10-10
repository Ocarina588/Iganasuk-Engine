#pragma once

#include <iostream>
#include "Context/Device.hpp"
#include "Image.hpp"

namespace iganasuk {

	class Swapchain {
	public:
		friend class RenderPass;
		
		Swapchain(void);
		~Swapchain(void);

		void init(Device::info_t &info, GLFWwindow* window, VkSurfaceKHR surface);
		VkExtent2D get_extent(GLFWwindow* window);
		void create_resources(void);
		void init_framebuffers(VkRenderPass render_pass);


		VkExtent2D extent;
		VkSurfaceFormatKHR format = { VK_FORMAT_B8G8R8A8_SRGB, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
	private:
		VkSwapchainKHR __ptr;

		std::vector <VkImage> __images;
		std::vector <VkImageView> __views;

		Image::handle __depth_image;

		std::vector <VkFramebuffer> __framebuffers;
		Device::info_t device_info;

	};
}