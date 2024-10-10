#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "utils.hpp"
#include "Synchronisation.hpp"

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
		VkFence					create_fence			(VkFenceCreateInfo& i);
		VkSemaphore				create_semaphore		(VkSemaphoreCreateInfo& i);
		VkShaderModule			create_shader_module	(VkShaderModuleCreateInfo& i);
		VkCommandPool			create_command_pool		(VkCommandPoolCreateInfo& i);
		VkPipelineLayout		create_pipeline_layout	(VkPipelineLayoutCreateInfo &i);
		VkPipeline				create_pipeline			(VkGraphicsPipelineCreateInfo& i);
		VkBuffer				create_buffer			(VkBufferCreateInfo &i);
		VkDescriptorPool		create_descriptor_pool	(VkDescriptorPoolCreateInfo &i);
		VkDescriptorSetLayout	create_descriptor_set_layout(VkDescriptorSetLayoutCreateInfo& i);

		void					destroy_image			(VkImage image)				{ vkDestroyImage(info.ptr, image, nullptr); };
		void					destroy_image_view		(VkImageView view)			{ vkDestroyImageView(info.ptr, view, nullptr); };
		void					destroy_framebuffer		(VkFramebuffer buffer)		{ vkDestroyFramebuffer(info.ptr, buffer, nullptr); }
		void					destroy_swapchain		(VkSwapchainKHR s)			{ vkDestroySwapchainKHR(info.ptr, s, nullptr); }
		void					destroy_render_pass		(VkRenderPass render_pass)	{ vkDestroyRenderPass(info.ptr, render_pass, nullptr); }
		void					destroy_fence			(VkFence f)					{ vkDestroyFence(info.ptr, f, nullptr); }
		void					destroy_semaphore		(VkSemaphore s)				{ vkDestroySemaphore(info.ptr, s, nullptr); }
		void					destroy_shader_module	(VkShaderModule s)			{ vkDestroyShaderModule(info.ptr, s, nullptr); }
		void					destroy_command_pool	(VkCommandPool c)			{ vkDestroyCommandPool(info.ptr, c, nullptr); }
		void					destroy_pipeline_layout	(VkPipelineLayout l)		{ vkDestroyPipelineLayout(info.ptr, l, nullptr); }
		void					destroy_pipeline		(VkPipeline p)				{ vkDestroyPipeline(info.ptr, p, nullptr); }
		void					destroy_buffer			(VkBuffer b)				{ vkDestroyBuffer(info.ptr, b, nullptr); }
		void					destroy_descriptor_pool (VkDescriptorPool p)		{ vkDestroyDescriptorPool(info.ptr, p, nullptr); }
		void					destroy_descriptor_set_layout(VkDescriptorSetLayout l) { vkDestroyDescriptorSetLayout(info.ptr, l, nullptr); }

		uint32_t				find_memory_type		(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		VkDeviceMemory			allocate_memory			(VkMemoryAllocateInfo& i);
		VkCommandBuffer			allocate_commad_buffer	(VkCommandBufferAllocateInfo& i);
		VkDescriptorSet			allocate_descriptor_set (VkDescriptorSetAllocateInfo &i);

		void					update_descriptor_set	(VkWriteDescriptorSet &i)	 { vkUpdateDescriptorSets(info.ptr, 1, &i, 0, nullptr); }

		void					map_memory				(VkDeviceMemory &m, VkDeviceSize size, void **data, VkMemoryMapFlags flags = 0, VkDeviceSize offset = 0);
		void					unmap_memory			(VkDeviceMemory &m) { vkUnmapMemory(info.ptr, m); }

		void					free_memory				(VkDeviceMemory i) { vkFreeMemory(info.ptr, i, nullptr); }
		void					free_command_buffer		(VkCommandBuffer i, VkCommandPool p) { vkFreeCommandBuffers(info.ptr, p, 1, &i); }
		void					free_descriptor_set		(VkDescriptorSet i, VkDescriptorPool p) { vkFreeDescriptorSets(info.ptr, p, 1, &i); }

		VkMemoryRequirements	get_memory_requirments	(VkBuffer buffer);
		VkMemoryRequirements	get_memory_requirments	(VkImage image);
		
		void					submit					(VkQueue queue, uint32_t n, VkSubmitInfo *submit_info, Fence &fence);
		void					present					(VkQueue queue, VkPresentInfoKHR &i) {vkQueuePresentKHR(queue, &i); }
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
