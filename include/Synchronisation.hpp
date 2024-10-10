#pragma once

#include <vulkan/vulkan.h>

namespace iganasuk {

	class Fence {
	public:
		Fence(bool signaled = true);
		~Fence(void);

		operator VkFence(void) { return __ptr; }
		operator VkFence*(void) { return &__ptr; }
		void wait(bool _reset = false);
		void reset(void);
	private:
		VkFence __ptr = nullptr;
	};

	class Semaphore {
	public:
		Semaphore(void);
		~Semaphore(void);

		operator VkSemaphore(void) { return __ptr; }
		operator VkSemaphore*(void) { return &__ptr; }
	private:
		VkSemaphore __ptr = nullptr;
	};
}