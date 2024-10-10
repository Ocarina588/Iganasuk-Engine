#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include "Buffer.hpp"

namespace iganasuk {

	namespace descriptor {
		class Pool {
		public:
			Pool(uint32_t descriptor_count, uint32_t max_sets, VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			~Pool(void);

			operator VkDescriptorPool(void) { return __ptr; }
		private:
			VkDescriptorPool __ptr = nullptr;
		};

		class Set {
		public:
			Set(VkDescriptorPool pool, uint32_t binding, VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uint32_t count = 1, VkShaderStageFlags flag = VK_SHADER_STAGE_VERTEX_BIT);
			~Set(void);
			operator VkDescriptorSet (void) { return __ptr; }
			operator VkDescriptorSet *(void) { return &__ptr; }
			operator VkDescriptorSetLayout (void) { return __layout; }
			operator VkDescriptorSetLayout *(void) { return &__layout; }

			void update(buffer::Uniform& buffer, uint32_t binding, VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		private:
			VkDescriptorSet __ptr = nullptr;
			VkDescriptorSetLayout __layout = nullptr;
			VkDescriptorPool __pool = nullptr;
		};
	}
}