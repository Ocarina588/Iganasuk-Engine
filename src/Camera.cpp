#include "Camera.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::Camera::Camera(void)
{

}

ig::Camera::~Camera(void)
{

}

glm::vec3 ig::Camera::rotate_A_over_B(glm::vec3 A, glm::vec3 B, glm::vec3 axis, float degrees)
{
	glm::vec3 nA = A - B;
	glm::mat4 rmat(1.f);
	rmat = glm::rotate(rmat, glm::radians(degrees), axis);;
	glm::vec4 nvec = rmat * glm::vec4(nA, 1.f);
	nA = glm::vec3(nvec);
	nA += B;
	return nA;
}

void ig::Camera::update(void)
{

}

void ig::Camera::process_input(ig::EventManager &e)
{
	float speed_move = ig::Context::get_dt() * 0.0001;
	float speed_rotate = ig::Context::get_dt() * 0.001;


	right = glm::normalize(glm::cross(world_up, direction));
	up = glm::cross(direction, right);

	if (e.W) pos += speed_move * direction ;
	if (e.S) pos -= speed_move * direction ;
	if (e.A) pos += speed_move * right;
	if (e.D) pos -= speed_move * right;
	if (e.E) pos += speed_move * world_up;
	if (e.Q) pos -= speed_move * world_up;

	if (e.r) direction = rotate_A_over_B(direction, pos, { 0.f, 0.f, 1.f }, -speed_rotate);
	if (e.l) direction = rotate_A_over_B(direction, pos, { 0.f, 0.f, 1.f }, +speed_rotate);
	if (e.u) direction = rotate_A_over_B(direction, pos, { 1.f, 0.f, 0.f }, +speed_rotate);
	if (e.d) direction = rotate_A_over_B(direction, pos, { 1.f, 0.f, 0.f }, -speed_rotate);

	target = pos + direction;
	view = glm::lookAt(pos, target, world_up);
}