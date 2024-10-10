#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.h>
#include <iostream>
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Command.hpp"
#include "Descriptor.hpp"

namespace iganasuk {

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;

		static shader::description_t get_description(void) {
			shader::description_t d;

			d.binding.binding = 0;
			d.binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			d.binding.stride = sizeof(Vertex);

			d.attributes.push_back({
				.location = 0,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(Vertex, pos)
				});
			d.attributes.push_back({
				.location = 1,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(Vertex, color)
				});

			return d;
		}
	};

	struct MVP {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class Mesh {
	public:
		Mesh(std::string const &name, void *data, uint32_t count, VkDeviceSize size);
		~Mesh(void);

		static Mesh* load(std::string const& file_name);
		static Mesh* load(std::string const& name, void* data, uint32_t count, VkDeviceSize size);
		buffer::Vertex	vbuffer;
		uint32_t		__count;
	private:
		std::string		__name;
		VkDeviceSize	__size;
	};

	class Model {
	public:
		Model(Mesh* _mesh, VkDescriptorPool pool, float scale = 1.f);

		void draw(CommandBuffer &cb) {
			VkDeviceSize offset = {};
			cb.bind_vertex_buffer(0, 1, __mesh->vbuffer, &offset);
			cb.draw(__mesh->__count, 1, 0, 0);
		}
		Mesh*		__mesh;
		glm::mat4	__model{ 1.f };
		buffer::Uniform ubuffer{ sizeof(MVP) };
		descriptor::Set descriptor_set;

	private:
	};

}