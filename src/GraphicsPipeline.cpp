#include "Pipeline.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::pipeline::Graphics::Graphics(
	std::vector<VkPipelineShaderStageCreateInfo> shader_stages,
	std::vector<shader::description_t> descriptions,
	std::vector<VkDescriptorSetLayout> layouts
)
{
	VkGraphicsPipelineCreateInfo create_info{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

	VkPipelineVertexInputStateCreateInfo vertex_input{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
	auto vertex_input_info = get_vertex_input_info(descriptions);
	vertex_input.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_input_info.attributes.size());
	vertex_input.vertexBindingDescriptionCount = static_cast<uint32_t>(vertex_input_info.bindings.size());
	vertex_input.pVertexAttributeDescriptions = vertex_input_info.attributes.data();
	vertex_input.pVertexBindingDescriptions = vertex_input_info.bindings.data();
	VkPipelineInputAssemblyStateCreateInfo input_assembly{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	VkPipelineViewportStateCreateInfo viewport_state{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
	VkViewport viewport{0.f, 0.f, static_cast<float>(ig_swapchain.extent.width), static_cast<float>(ig_swapchain.extent.height), 0.f, 1.f};
	VkRect2D scissors{ {}, ig_swapchain.extent };
	viewport_state.scissorCount = 1;
	viewport_state.viewportCount = 1;
	viewport_state.pScissors = &scissors;
	viewport_state.pViewports = &viewport;
	VkPipelineRasterizationStateCreateInfo rasterization{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
	rasterization.polygonMode = VK_POLYGON_MODE_FILL;
	rasterization.cullMode = VK_CULL_MODE_NONE;
	rasterization.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterization.lineWidth = 1.f;
	VkPipelineMultisampleStateCreateInfo multisample{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
	multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	VkPipelineDepthStencilStateCreateInfo depth_stencil{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
	depth_stencil.depthTestEnable = VK_TRUE;
	depth_stencil.depthWriteEnable = VK_TRUE;
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
	VkPipelineColorBlendAttachmentState attachment{};
	attachment.colorWriteMask = 0b1111;
	VkPipelineColorBlendStateCreateInfo color_blend{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO }; 
	color_blend.attachmentCount = 1;
	color_blend.pAttachments = &attachment;
	VkPipelineLayoutCreateInfo pipeline_layout_create_info{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO }; 
	pipeline_layout_create_info.setLayoutCount = static_cast<uint32_t>(layouts.size());
	pipeline_layout_create_info.pSetLayouts = layouts.data();
	__layout = ig_device.create_pipeline_layout(pipeline_layout_create_info);

	create_info = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, nullptr, 0, 
		static_cast<uint32_t>(shader_stages.size()), shader_stages.data(),
		&vertex_input,
		&input_assembly,
		nullptr, 
		&viewport_state,
		&rasterization,
		&multisample,
		&depth_stencil,
		&color_blend,
		nullptr,
		__layout,
		ig_render_pass,
		0
	};

	__ptr = ig_device.create_pipeline(create_info);
}

ig::pipeline::Graphics::~Graphics(void)
{
	ig_device.destroy_pipeline_layout(__layout);
	ig_device.destroy_pipeline(__ptr);
}

ig::pipeline::Graphics::vertex_input_info_t ig::pipeline::Graphics::get_vertex_input_info(std::vector<shader::description_t>& descriptions)
{
	vertex_input_info_t info;
	
	for (auto i : descriptions) {
			info.bindings.push_back(i.binding);
		for (auto j : i.attributes)
			info.attributes.push_back(j);
	}

	return info;
}
