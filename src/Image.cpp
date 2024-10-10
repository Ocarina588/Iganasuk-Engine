#include "Image.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::Image::Image(VkExtent2D extent, VkFormat f, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspect)
{
    format  =   f;
    ptr     =   create_image(extent, format, tiling, usage);
    memory  =   create_memory(ptr, properties);
    view    =   create_view(ptr, format, aspect);
}

ig::Image::~Image(void)
{
    ig_device.destroy_image_view(view);
    ig_device.destroy_image(ptr);
    ig_device.free_memory(memory);
}

VkImage ig::Image::create_image(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = extent.width;
    imageInfo.extent.height = extent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    return ig_device.create_image(imageInfo);
}

VkDeviceMemory ig::Image::create_memory(VkImage image, VkMemoryPropertyFlags properties)
{
    VkMemoryRequirements memRequirements = ig_device.get_memory_requirments(image);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = ig_device.find_memory_type(memRequirements.memoryTypeBits, properties);

    VkDeviceMemory memory = ig_device.allocate_memory(allocInfo);

    ig_device.bind_memory(image, memory);

    return memory;
}

VkImageView ig::Image::create_view(VkImage image, VkFormat format, VkImageAspectFlags aspect)
{
    VkImageViewCreateInfo create_info{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    create_info.image = image;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;
    create_info.subresourceRange.aspectMask = aspect;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.layerCount = 1;
    create_info.subresourceRange.levelCount = 1;

    return ig_device.create_image_view(create_info);
}