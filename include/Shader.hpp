#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

namespace iganasuk {
	namespace shader {
		class __ShaderConstructor {
			friend class Vertex;
			friend class Fragment;

			__ShaderConstructor(std::string const& file_name, VkShaderStageFlagBits flags);
			~__ShaderConstructor(void);

			VkShaderModule module = nullptr;
			VkPipelineShaderStageCreateInfo stage{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		public:
			operator VkPipelineShaderStageCreateInfo(void) { return stage; }
		};

		class Vertex : public __ShaderConstructor {
		public:
			Vertex(std::string const& file_name) : __ShaderConstructor(file_name, VK_SHADER_STAGE_VERTEX_BIT) {};
			~Vertex(void) {}
		};

		class Fragment : public __ShaderConstructor {
		public:
			Fragment(std::string const& file_name) :__ShaderConstructor(file_name, VK_SHADER_STAGE_FRAGMENT_BIT) {};
			~Fragment(void) {}
		};

		typedef struct description_s {
			VkVertexInputBindingDescription binding;
			std::vector<VkVertexInputAttributeDescription> attributes;
		} description_t;
	}
}