#pragma once

#include <vulkan/vulkan.h>
#include <iostream>

namespace iganasuk {

	class Image {
	public:
		using handle = std::shared_ptr<Image>;
		static handle create(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
			VkMemoryPropertyFlags properties, VkImageAspectFlags aspect) {
			return handle(new Image(extent, format, tiling, usage, properties, aspect));
		}

		~Image(void);

		static VkImage			create_image(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
		static VkDeviceMemory	create_memory(VkImage image, VkMemoryPropertyFlags properties);
		static VkImageView		create_view(VkImage image, VkFormat format, VkImageAspectFlags aspect);

		operator VkImage (void) { return ptr; };
		operator VkImageView (void) { return view; };
		operator VkDeviceMemory (void) { return memory; };


		VkImage ptr = nullptr;
		VkImageView view = nullptr;
		VkDeviceMemory memory = nullptr;
		VkFormat format{};
	private:
		Image(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
			VkMemoryPropertyFlags properties, VkImageAspectFlags aspect);
	};
}