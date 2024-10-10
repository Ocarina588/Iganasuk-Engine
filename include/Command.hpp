#pragma once

#include <vulkan/vulkan.h>

namespace iganasuk {
	namespace CommandPool {
		class __CommandPoolConstructor {
			friend class Reset;

			__CommandPoolConstructor(uint32_t index_queue, VkCommandPoolCreateFlags flags);
			~__CommandPoolConstructor(void);

			VkCommandPool __ptr;
		public:
			operator VkCommandPool(void) { return __ptr; }
		};

		class Reset : public __CommandPoolConstructor {
		public:
			Reset(uint32_t index_queue) : __CommandPoolConstructor(index_queue, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {}
			~Reset(void) {}
		private:

		};
	}

	class CommandBuffer {
	public:
		CommandBuffer(VkCommandPool pool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~CommandBuffer(void);

		operator VkCommandBuffer (void) { return __ptr; }
		operator VkCommandBuffer* (void) { return &__ptr; }

		void reset					(void) { vkResetCommandBuffer(__ptr, 0); }
		void begin					(void);
		void begin					(VkCommandBufferBeginInfo& i);
		void end					(void);
		void begin_render_pass		(VkRenderPassBeginInfo& i, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) { vkCmdBeginRenderPass(__ptr, &i, contents); }
		void end_render_pass		(void) { vkCmdEndRenderPass(__ptr); }
		void bind_pipeline_graphics	(VkPipeline pipeline) { vkCmdBindPipeline(__ptr, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline); }
		void bind_vertex_buffer		(uint32_t first_binding, uint32_t binding_count, VkBuffer* buffer, VkDeviceSize* offset) { vkCmdBindVertexBuffers(__ptr, first_binding, binding_count, buffer, offset); }
		void draw					(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) { vkCmdDraw(__ptr, vertex_count, instance_count, first_vertex, first_instance); }
		void bind_descriptior_sets	(VkPipelineBindPoint bind_point, VkPipelineLayout layout, uint32_t first_set, uint32_t count, VkDescriptorSet* s, uint32_t offset_count = 0, uint32_t* d_o = nullptr) { vkCmdBindDescriptorSets(__ptr, bind_point, layout, first_set, count, s, offset_count, d_o); }

	private:
		VkCommandBuffer __ptr;
		VkCommandPool __pool;
	};
}
