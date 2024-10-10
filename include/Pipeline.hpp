#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "Shader.hpp"

namespace iganasuk {
	namespace pipeline {
		class Graphics {
		public:
			Graphics(
				std::vector<VkPipelineShaderStageCreateInfo> shader_stages, 
				std::vector<shader::description_t> descriptions,
				std::vector<VkDescriptorSetLayout> layouts
			);
			~Graphics(void);

			operator VkPipeline(void) { return __ptr; }
			operator VkPipelineLayout(void) { return __layout; }
		private:
			typedef struct vertex_input_info_s {
				std::vector<VkVertexInputBindingDescription> bindings;
				std::vector<VkVertexInputAttributeDescription> attributes;
			} vertex_input_info_t;
			vertex_input_info_t get_vertex_input_info(std::vector<shader::description_t>& descriptions);
			VkPipeline __ptr;
			VkPipelineLayout __layout;
		};
	}
}