#include "Synchronisation.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::Fence::Fence(bool signaled)
{
	VkFenceCreateInfo create_info{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	//create_info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
	create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	__ptr = ig_device.create_fence(create_info);
}

ig::Fence::~Fence(void)
{
	ig_device.destroy_fence(__ptr);
}

ig::Semaphore::Semaphore(void)
{
	VkSemaphoreCreateInfo create_info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	__ptr = ig_device.create_semaphore(create_info);
}

ig::Semaphore::~Semaphore(void)
{
	ig_device.destroy_semaphore(__ptr);
}

void ig::Fence::wait(bool r)
{
	vkWaitForFences(ig_device, 1, &__ptr, VK_TRUE, UINT64_MAX);
	if (r)
		reset();
}

void ig::Fence::reset(void)
{
	vkResetFences(ig_device, 1, &__ptr);
}