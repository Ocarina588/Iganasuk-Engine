#pragma once 

#include <vulkan/vulkan.h>
#include <vector>

namespace iganasuk {

	namespace buffer {
		class __BufferCreator {
			friend class Vertex;
			friend class Uniform;

			__BufferCreator(void *data, VkDeviceSize size, VkBufferUsageFlags usage);
			~__BufferCreator(void);

			VkBuffer __ptr = nullptr;
			VkDeviceMemory __memory = nullptr;
		public:
			operator VkBuffer(void) { return __ptr; }
			operator VkBuffer *(void) { return &__ptr; }
		};

		class Vertex : public __BufferCreator {
		public:
			Vertex(void *data, VkDeviceSize size) : __BufferCreator(data, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {}
			~Vertex(void) {};
			operator VkBuffer(void) { return __ptr; }
			operator VkBuffer* (void) { return &__ptr; }
		private:
		};

		class Uniform : public __BufferCreator {
		public:
			Uniform(VkDeviceSize size);
			~Uniform(void) {};

			void memcpy(void* d, size_t size = 0);

			operator VkDeviceSize(void) { return static_cast<VkDeviceSize>(__size); }
			operator VkBuffer(void) { return __ptr; }
			operator VkBuffer* (void) { return &__ptr; }
		private:
			void* __data = nullptr;
			size_t __size = 0;
		};

	}
}
