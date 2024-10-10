#include "Buffer.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::buffer::__BufferCreator::__BufferCreator(void* data, VkDeviceSize size, VkBufferUsageFlags usage)
{
	VkBufferCreateInfo create_info{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	create_info.size = size;
	create_info.usage = usage;
	create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	__ptr = ig_device.create_buffer(create_info);

	auto mem_requirements = ig_device.get_memory_requirments(__ptr);
	VkMemoryAllocateInfo memory_info{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	memory_info.allocationSize = mem_requirements.size;
	memory_info.memoryTypeIndex = ig_device.find_memory_type(mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	__memory = ig_device.allocate_memory(memory_info);

	ig_device.bind_memory(__ptr, __memory);

	if (data == nullptr) return;

	void* tmp = nullptr;
	ig_device.map_memory(__memory, size, &tmp);
	std::memcpy(tmp, data, static_cast<size_t>(size));
	ig_device.unmap_memory(__memory);
}

ig::buffer::__BufferCreator::~__BufferCreator(void)
{
	ig_device.free_memory(__memory);
	ig_device.destroy_buffer(__ptr);
}

ig::buffer::Uniform::Uniform(VkDeviceSize size) : __BufferCreator(nullptr, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
{
	ig_device.map_memory(__memory, size, &__data);
	__size = static_cast<size_t>(size);
}

void ig::buffer::Uniform::memcpy(void* d, size_t size)
{
	if (size == 0)
		size = __size;
	std::memcpy(__data, d, size);
}