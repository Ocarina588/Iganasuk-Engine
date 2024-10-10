#include "Model.hpp"
#include "objloader.hpp"

namespace ig = iganasuk;

ig::Mesh::Mesh(std::string const& name, void* data, uint32_t count, VkDeviceSize size) : 
	vbuffer(data, size * count), __name(name), __count(count), __size(size)
{

}

ig::Mesh *ig::Mesh::load(std::string const& file_name)
{
	std::vector<ig::Vertex> vertices;
	objl::Loader loader;
	
	bool l = loader.LoadFile(file_name);
	if (!l)
		throw std::runtime_error("failed to load: " + file_name);

	auto& mesh = loader.LoadedMeshes[0];
	std::vector<glm::vec3> ns(mesh.Indices.size());
	for (int j = 0; j < mesh.Indices.size(); j += 3) {
		glm::vec3* a = (glm::vec3*)&mesh.Vertices[mesh.Indices[j]].Position;
		glm::vec3* b = (glm::vec3*)&mesh.Vertices[mesh.Indices[j + 1]].Position;
		glm::vec3* c = (glm::vec3*)&mesh.Vertices[mesh.Indices[j + 2]].Position;
		glm::vec3 n = glm::normalize(glm::cross(*b - *a, *c - *a));
		ns[mesh.Indices[j]] += n;
		ns[mesh.Indices[j + 1]] += n;
		ns[mesh.Indices[j + 2]] += n;

		vertices.push_back({ {a->x, a->y, a->z},{} });
		vertices.push_back({ {b->x, b->y, b->z},{} });
		vertices.push_back({ {c->x, c->y, c->z},{} });
	}
	for (auto& i : ns)
		i = glm::normalize(i);
	for (int i = 0; i < mesh.Indices.size(); i++)
		vertices[i].color = ns[mesh.Indices[i]];
	
	return new Mesh(file_name, vertices.data(), static_cast<uint32_t>(vertices.size()), static_cast<VkDeviceSize>(sizeof(ig::Vertex)));
}

ig::Mesh* ig::Mesh::load(std::string const& name, void* data, uint32_t count, VkDeviceSize size)
{
	return new Mesh(name, data, count, size);
}

ig::Model::Model(ig::Mesh* _mesh, VkDescriptorPool pool, float scale) : descriptor_set(pool, 0)
{
	__mesh = _mesh;
	__model = glm::rotate(__model, glm::radians(90.f), { 1.f, 0.f, 0.f });
	__model = glm::scale(__model, glm::vec3(scale));

	descriptor_set.update(ubuffer, 0);
}