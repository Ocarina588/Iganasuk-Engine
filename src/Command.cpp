#include "Command.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::CommandPool::__CommandPoolConstructor::__CommandPoolConstructor(uint32_t index_queue, VkCommandPoolCreateFlags flags)
{
	VkCommandPoolCreateInfo create_info{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };

	create_info.queueFamilyIndex = index_queue;
	create_info.flags = flags;

	__ptr = ig_device.create_command_pool(create_info);

}

ig::CommandPool::__CommandPoolConstructor::~__CommandPoolConstructor(void)
{
	ig_device.destroy_command_pool(__ptr);
}

ig::CommandBuffer::CommandBuffer(VkCommandPool pool, VkCommandBufferLevel level)
{
	VkCommandBufferAllocateInfo alloc_info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	alloc_info.commandPool = pool;
	alloc_info.commandBufferCount = 1;
	alloc_info.level = level;

	__ptr = ig_device.allocate_commad_buffer(alloc_info);
	__pool = pool;
}

ig::CommandBuffer::~CommandBuffer(void)
{
	ig_device.free_command_buffer(__ptr, __pool);
}

void ig::CommandBuffer::begin(void)
{
	VkCommandBufferBeginInfo info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	begin(info);
}

void ig::CommandBuffer::begin(VkCommandBufferBeginInfo& i)
{
	if (vkBeginCommandBuffer(__ptr, &i) != VK_SUCCESS)
		throw std::runtime_error("failed to begin command buffer");
}

void ig::CommandBuffer::end(void)
{
	if (vkEndCommandBuffer(__ptr) != VK_SUCCESS)
		throw std::runtime_error("failed to record command buffer");
}