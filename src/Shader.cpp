#include "Shader.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::shader::__ShaderConstructor::__ShaderConstructor(std::string const& file_name, VkShaderStageFlagBits flag)
{
	auto data = utils::readFile(file_name);
	VkShaderModuleCreateInfo create_info{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
	create_info.codeSize = static_cast<uint32_t>(data.size());
	create_info.pCode = reinterpret_cast<uint32_t*>(data.data());

	module = ig_device.create_shader_module(create_info);

	stage.module = module;
	stage.pName = "main";
	stage.stage = flag;
}

ig::shader::__ShaderConstructor::~__ShaderConstructor(void)
{
	ig_device.destroy_shader_module(module);
}

