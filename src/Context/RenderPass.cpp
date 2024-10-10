#include <array>
#include "Context.hpp"

namespace ig = iganasuk;

ig::RenderPass::RenderPass(void)
{

}

ig::RenderPass::~RenderPass(void)
{
	ig_device.destroy_render_pass(__ptr);
}

void ig::RenderPass::init()
{
	VkAttachmentDescription color_attachment{};

	color_attachment.format = ig_swapchain.format.format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depth_attachment{};

	depth_attachment.format = ig_swapchain.__depth_image->format;
	depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference color_ref{};  
	color_ref.attachment = 0; 
	color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 

	VkAttachmentReference depth_ref{};
	depth_ref.attachment = 1;
	depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass_description{};
	subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_ref;
	subpass_description.pDepthStencilAttachment = &depth_ref;

	VkSubpassDependency subpass_dependency{};
	subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.srcAccessMask = 0;

	subpass_dependency.dstSubpass = 0;
	subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { color_attachment, depth_attachment };
	std::array<VkSubpassDescription, 1> subpasses = { subpass_description };
	std::array<VkSubpassDependency, 1> dependencies = { subpass_dependency };

	VkRenderPassCreateInfo create_info{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
	create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	create_info.subpassCount = static_cast<uint32_t>(subpasses.size());
	create_info.dependencyCount = static_cast<uint32_t>(dependencies.size());
	create_info.pAttachments = attachments.data();
	create_info.pSubpasses = subpasses.data();
	create_info.pDependencies = dependencies.data();

	__ptr = ig_device.create_renderpass(create_info);
}