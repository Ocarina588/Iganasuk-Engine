#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Event.hpp"

namespace iganasuk {

	class Camera {
	public:
		Camera(void);
		~Camera(void);

		void process_input(EventManager &e);
		void update(void);
		glm::vec3 rotate_A_over_B(glm::vec3 A, glm::vec3 B, glm::vec3 axis, float degrees);

		glm::vec3 world_up = glm::vec3(0.f, 0.f, 1.f);
		glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);
		glm::vec3 right = glm::normalize(glm::cross(up, direction));

		glm::vec3 pos{ 0.f, -2.f, 0.f };
		glm::vec3 direction{0.f, 1.f, 0.f};
		glm::vec3 target{ 0.f, 1.f, 0.f };
		glm::mat4 view = glm::lookAt(pos, pos + direction, up);

	private:
	};
}
