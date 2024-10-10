#include <unordered_set>
#include "Context/Device.hpp"

namespace ig = iganasuk;

ig::Device::Device(void)
{

}

ig::Device::~Device(void)
{
	vkDestroyDevice(info.ptr, nullptr);
}

void ig::Device::add_extension(char const* extension)
{
	__extensions.push_back(extension);
}

void ig::Device::init(VkInstance instance, VkSurfaceKHR surface)
{
	uint32_t count = 0;
	float priority = 1.f;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);
	std::vector<VkPhysicalDevice> physical_devices(count);
	vkEnumeratePhysicalDevices(instance, &count, physical_devices.data());

	for (auto i : physical_devices) {
		auto tmp = get_physical_info(i, surface);
		if (suitable(tmp)) {
			info = tmp;
			info.physical_ptr = i;
			break;
		}
	}
	if (info.physical_ptr == nullptr)
		throw std::runtime_error("failed to find suitable device");
	
	std::unordered_set<uint32_t> queue_indices{ info.index_compute, info.index_graphics, info.index_present, info.index_transfer };
	std::vector<VkDeviceQueueCreateInfo> queue_create_info;
	for (auto i : queue_indices) {
		queue_create_info.push_back({
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = i,
			.queueCount = 1,
			.pQueuePriorities = &priority
		});
	}


	VkDeviceCreateInfo create_info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	create_info.enabledExtensionCount = static_cast<uint32_t>(__extensions.size());
	create_info.ppEnabledExtensionNames = __extensions.data();
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_info.size());
	create_info.pQueueCreateInfos = queue_create_info.data();

	if (vkCreateDevice(info.physical_ptr, &create_info, nullptr, &info.ptr) != VK_SUCCESS)
		throw std::runtime_error("failed to create device");

	vkGetDeviceQueue(info.ptr, info.index_compute, 0, &queue_compute);
	vkGetDeviceQueue(info.ptr, info.index_graphics, 0, &queue_graphics);
	vkGetDeviceQueue(info.ptr, info.index_present, 0, &queue_present);
	vkGetDeviceQueue(info.ptr, info.index_transfer, 0, &queue_transfer);
}

ig::Device::info_t ig::Device::get_physical_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
	info_t tmp{};
	uint32_t count = 0;

	vkGetPhysicalDeviceProperties(physical_device, &tmp.properties);
	vkGetPhysicalDeviceFeatures(physical_device, &tmp.featues);
	vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, nullptr);
	tmp.extensions.resize(count);
	vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, tmp.extensions.data());

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &tmp.surface_capabilites);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &count, nullptr);
	tmp.present_modes.resize(count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &count, tmp.present_modes.data());
	vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &count, nullptr);
	tmp.surface_formats.resize(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &count, tmp.surface_formats.data());

	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);
	tmp.queue_family_properties.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, tmp.queue_family_properties.data());

	for (uint32_t i = 0; i < count ; i++) {
		if (tmp.index_compute == INVALID_UINT32 && tmp.queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			tmp.index_compute = i;
		if (tmp.index_graphics == INVALID_UINT32 && tmp.queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			tmp.index_graphics = i;
		if (tmp.index_transfer == INVALID_UINT32 && tmp.queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			tmp.index_transfer = i;

		VkBool32 supported = false;
		if (vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &supported) != VK_SUCCESS)
			throw std::runtime_error("failed to fetch surface support");

		if (supported && tmp.index_present == INVALID_UINT32)
			tmp.index_present = i;
	}

	return tmp;
}

bool ig::Device::suitable(info_t const& inf)
{
	if (inf.properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) return false;
	if (inf.index_compute == INVALID_UINT32 || inf.index_graphics == INVALID_UINT32 ||
		inf.index_present == INVALID_UINT32 || inf.index_transfer == INVALID_UINT32 ||
		inf.featues.geometryShader == false)
		return false;
	return true;
}

uint32_t ig::Device::find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(info.physical_ptr, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i; 

	throw std::runtime_error("failed to find suitable memory type!");
}

VkImage ig::Device::create_image(VkImageCreateInfo &i)
{
	VkImage image = nullptr;
	if (vkCreateImage(info.ptr, &i, nullptr, &image) != VK_SUCCESS)
		throw std::runtime_error("failed to create image");
	return image;
}

VkImageView ig::Device::create_image_view(VkImageViewCreateInfo& i)
{
	VkImageView view = nullptr;
	if (vkCreateImageView(info.ptr, &i, nullptr, &view) != VK_SUCCESS)
		throw std::runtime_error("failed to create image");
	return view;
}

VkFramebuffer ig::Device::create_framebuffer(VkFramebufferCreateInfo& i)
{
	VkFramebuffer buffer = nullptr;
	if (vkCreateFramebuffer(info.ptr, &i, nullptr, &buffer) != VK_SUCCESS)
		throw std::runtime_error("failed to create framebuffer");
	return buffer;
}

VkDeviceMemory ig::Device::allocate_memory(VkMemoryAllocateInfo& i)
{
	VkDeviceMemory memory = nullptr;
	if (vkAllocateMemory(info.ptr, &i, nullptr, &memory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate memory");
	return memory;
}

VkCommandBuffer ig::Device::allocate_commad_buffer(VkCommandBufferAllocateInfo& i)
{
	VkCommandBuffer cb = nullptr;
	if (vkAllocateCommandBuffers(info.ptr, &i, &cb) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffer");
	return cb;
}

VkDescriptorSet ig::Device::allocate_descriptor_set(VkDescriptorSetAllocateInfo& i)
{
	VkDescriptorSet s = nullptr;
	if (vkAllocateDescriptorSets(info.ptr, &i, &s) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor set");
	return s;
}


VkSwapchainKHR ig::Device::create_swapchain(VkSwapchainCreateInfoKHR& i)
{
	VkSwapchainKHR s = nullptr;
	if (vkCreateSwapchainKHR(info.ptr, &i, nullptr, &s) != VK_SUCCESS)
		throw std::runtime_error("failed to create swapchain");
	return s;
}

VkRenderPass ig::Device::create_renderpass(VkRenderPassCreateInfo& i)
{
	VkRenderPass r = nullptr;
	if (vkCreateRenderPass(info.ptr, &i, nullptr, &r) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass");
	return r;
}

VkFence ig::Device::create_fence(VkFenceCreateInfo& i)
{
	VkFence f = nullptr;
	if (vkCreateFence(info.ptr, &i, nullptr, &f) != VK_SUCCESS)
		throw std::runtime_error("failed to create fence");
	return f;
}

VkSemaphore ig::Device::create_semaphore(VkSemaphoreCreateInfo& i)
{
	VkSemaphore s = nullptr;
	if (vkCreateSemaphore(info.ptr, &i, nullptr, &s) != VK_SUCCESS)
		throw std::runtime_error("failed to create semaphore");
	return s;
}

VkShaderModule ig::Device::create_shader_module(VkShaderModuleCreateInfo& i)
{
	VkShaderModule m = nullptr;
	if (vkCreateShaderModule(info.ptr, &i, nullptr, &m) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module");
	return m;
}

VkCommandPool ig::Device::create_command_pool(VkCommandPoolCreateInfo& i)
{
	VkCommandPool p = nullptr;
	if (vkCreateCommandPool(info.ptr, &i, nullptr, &p) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool");
	return p;
}

VkPipelineLayout ig::Device::create_pipeline_layout(VkPipelineLayoutCreateInfo& i)
{
	VkPipelineLayout l = nullptr;
	if (vkCreatePipelineLayout(info.ptr, &i, nullptr, &l) != VK_SUCCESS)
		throw std::runtime_error("failed to create layout");
	return l;
}

VkPipeline ig::Device::create_pipeline(VkGraphicsPipelineCreateInfo& i)
{
	VkPipeline g = nullptr;
	if (vkCreateGraphicsPipelines(info.ptr, nullptr, 1, &i, nullptr, &g) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline");
	return g;
}

VkBuffer ig::Device::create_buffer(VkBufferCreateInfo& i)
{
	VkBuffer b = nullptr;
	if (vkCreateBuffer(info.ptr, &i, nullptr, &b) != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer");
	return b;
}

VkDescriptorPool ig::Device::create_descriptor_pool(VkDescriptorPoolCreateInfo& i)
{
	VkDescriptorPool p = nullptr;
	if (vkCreateDescriptorPool(info.ptr, &i, nullptr, &p) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor pool");
	return p;
}

VkDescriptorSetLayout ig::Device::create_descriptor_set_layout(VkDescriptorSetLayoutCreateInfo& i)
{
	VkDescriptorSetLayout l = nullptr;
	if (vkCreateDescriptorSetLayout(info.ptr, &i, nullptr, &l) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor layout");
	return l;
}

void ig::Device::bind_memory(VkImage image, VkDeviceMemory memory, VkDeviceSize offset)
{
	vkBindImageMemory(info.ptr, image, memory, offset);
}

void ig::Device::bind_memory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize offset)
{
	vkBindBufferMemory(info.ptr, buffer, memory, offset);
}

VkMemoryRequirements ig::Device::get_memory_requirments(VkImage image)
{
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(info.ptr, image, &memRequirements);
	return memRequirements;
}

VkMemoryRequirements ig::Device::get_memory_requirments(VkBuffer buffer)
{
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(info.ptr, buffer, &memRequirements);
	return memRequirements;
}

void ig::Device::submit(VkQueue queue, uint32_t n, VkSubmitInfo* info, ig::Fence& fence)
{
	if (vkQueueSubmit(queue, n, info, fence) != VK_SUCCESS)
		throw std::runtime_error("failed to submit to queue");
}

void ig::Device::map_memory(VkDeviceMemory& m, VkDeviceSize size, void** data, VkMemoryMapFlags flags, VkDeviceSize offset)
{
	if (vkMapMemory(info.ptr, m, offset, size, flags, data) != VK_SUCCESS)
		throw std::runtime_error("failed to map memory");
}