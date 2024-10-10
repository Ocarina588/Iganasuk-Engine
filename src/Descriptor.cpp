#include "Descriptor.hpp"
#include "Context.hpp"

namespace ig = iganasuk::descriptor;

ig::Pool::Pool(uint32_t descriptor_count, uint32_t max_sets, VkDescriptorType type)
{
	VkDescriptorPoolCreateInfo create_info{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

	VkDescriptorPoolSize pool_size{};
	pool_size.type = type;
	pool_size.descriptorCount = descriptor_count;

	create_info.maxSets = max_sets;
	create_info.poolSizeCount = 1;
	create_info.pPoolSizes = &pool_size;

	__ptr = ig_device.create_descriptor_pool(create_info);
}

ig::Pool::~Pool(void)
{
	ig_device.destroy_descriptor_pool(__ptr);
}

ig::Set::Set(VkDescriptorPool pool, uint32_t b, VkDescriptorType type, uint32_t count, VkShaderStageFlags flag)
{
	VkDescriptorSetLayoutBinding binding{};
	binding.binding = b;
	binding.descriptorType = type;
	binding.descriptorCount = count;
	binding.stageFlags = flag;

	VkDescriptorSetLayoutCreateInfo layout_info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	layout_info.bindingCount = 1;
	layout_info.pBindings = &binding;

	__layout = ig_device.create_descriptor_set_layout(layout_info);

	VkDescriptorSetAllocateInfo alloc_info{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
	alloc_info.descriptorPool = pool;
	alloc_info.descriptorSetCount = 1;
	alloc_info.pSetLayouts = &__layout;

	__ptr = ig_device.allocate_descriptor_set(alloc_info);
	__pool = pool;
}

ig::Set::~Set(void)
{
	ig_device.destroy_descriptor_set_layout(__layout);
}

void ig::Set::update(buffer::Uniform& buffer, uint32_t binding, VkDescriptorType type)
{
	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = buffer;
	buffer_info.offset = 0;
	buffer_info.range = buffer;

	VkWriteDescriptorSet descriptor_write{};
	descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write.dstSet = __ptr;
	descriptor_write.dstBinding = binding;
	descriptor_write.dstArrayElement = 0;
	descriptor_write.descriptorType = type;
	descriptor_write.descriptorCount = 1;
	descriptor_write.pBufferInfo = &buffer_info;

	ig_device.update_descriptor_set(descriptor_write);
}